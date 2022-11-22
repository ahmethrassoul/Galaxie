/*
 *  Copyright (c) 2022 Bertrand LE GAL
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */
 #include <metal_stdlib>
using namespace metal;

constant int THREADGROUP_SIZE = 1024; // seems to be a constant !
/*
 *
 *
 *##############################################################################
 *
 *
 */
#define BSIZE 256 			   // Block size in # of threads
#define softeningSquared 0.01f // original plumer softener is 0.025. here the value is square of it.
#define damping          1.00f // 0.999f
#define ep               0.67f // 0.5f
/*
 *
 *
 *##############################################################################
 *
 *
 */
 kernel void body2Body_copy_v2(
              device float4* c_pos,
        const device float4* n_pos,
        const device float4* v_dat,
        const device uint*   nElements,
        uint    tid      [[ thread_index_in_threadgroup  ]],
        uint    bid      [[ threadgroup_position_in_grid ]],
        uint    blockDim [[ threads_per_threadgroup      ]]
        )
{
    uint particule_id  = bid * blockDim + tid;
    uint half_e        = nElements[0] / 2;
    if( particule_id < half_e )
    {
        c_pos[ particule_id          ] = n_pos[ particule_id          ];
        c_pos[ particule_id + half_e ] = n_pos[ particule_id + half_e ];
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
kernel void body2Body_exec_v2(
        const device float4* c_pos,
              device float4* n_pos,
              device float4* v_dat,
        const device uint*   nElements,
        uint    tid      [[ thread_index_in_threadgroup  ]],
        uint    bid      [[ threadgroup_position_in_grid ]],
        uint    blockDim [[ threads_per_threadgroup      ]]
        )
{

    threadgroup float4 shPosition[THREADGROUP_SIZE];

    const uint max_particules = nElements[0];                       // number of particules
    const uint off_particules = max_particules / 2;                 // half particule
    const uint particule_id_1 = bid * blockDim + tid;               // the 1st particule offset
    const uint particule_id_2 = particule_id_1 + off_particules;    // the 2nd particule offset

    threadgroup_barrier(mem_flags::mem_none);

    //
    // Si jamais a cause des threads/grilles ont deborde...
    //

    if (particule_id_1 >= off_particules ) // on filtre sur N/2 particule car chaque coeur traite 2 particules !
    {
        return;
    }

    //
    // Dans le cas contraire on commence notre calcul !
    //

    float4 myPosition_1 = c_pos[ particule_id_1 ];  // la particule 1
    float4 myPosition_2 = c_pos[ particule_id_2 ];  // la particule 2 décalée de (max_particules/2)

    threadgroup_barrier(mem_flags::mem_none);

    //
    //
    //

    float3 myAccel_1 = {0.f, 0.f, 0.f};
    float3 myAccel_2 = {0.f, 0.f, 0.f};

    for (uint idx = 0; idx < max_particules; idx += blockDim)
    {
        shPosition[tid] = c_pos[idx + tid];        // prefetching in local memory
        threadgroup_barrier(mem_flags::mem_none);  // prefetching in local memory

        for (uint p = 0; p < blockDim; p += 1)
        {
            const float4 other = shPosition[p];

            float rx_1 = other.x - myPosition_1.x, rx_2 = other.x - myPosition_2.x;
            float ry_1 = other.y - myPosition_1.y, ry_2 = other.y - myPosition_2.y;
            float rz_1 = other.z - myPosition_1.z, rz_2 = other.z - myPosition_2.z;

            float d_1 = (rx_1 * rx_1) + (ry_1 * ry_1) + (rz_1 * rz_1);
            float d_2 = (rx_2 * rx_2) + (ry_2 * ry_2) + (rz_2 * rz_2);

            float d3_1;
            if ( d_1 < 1.0f ) d3_1 = (10.0f * other.w                    );
            else              d3_1 = (10.0f * other.w / (sqrt(d_1) * d_1));

            float d3_2;
            if ( d_2 < 1.0f ) d3_2 = (10.0f * other.w                    );
            else              d3_2 = (10.0f * other.w / (sqrt(d_2) * d_2));

            myAccel_1.x += rx_1 * d3_1; myAccel_2.x += rx_2 * d3_2;
            myAccel_1.y += ry_1 * d3_1; myAccel_2.y += ry_2 * d3_2;
            myAccel_1.z += rz_1 * d3_1; myAccel_2.z += rz_2 * d3_2;

        }
    }

    //
    // update position
    //

    threadgroup_barrier(mem_flags::mem_none);

    float4 myVelocity_1    = v_dat[ particule_id_1 ];
           myVelocity_1.x += myAccel_1.x * 2.0f;
           myVelocity_1.y += myAccel_1.y * 2.0f;
           myVelocity_1.z += myAccel_1.z * 2.0f;
           myPosition_1.x  = myPosition_1.x + (myVelocity_1.x * 0.01f);
           myPosition_1.y  = myPosition_1.y + (myVelocity_1.y * 0.01f);
           myPosition_1.z  = myPosition_1.z + (myVelocity_1.z * 0.01f);

    threadgroup_barrier(mem_flags::mem_none);

    n_pos[ particule_id_1 ] = myPosition_1;     // NEXT POSITION !
    v_dat[ particule_id_1 ] = myVelocity_1;     // NEW  VELOCITY !

    float4 myVelocity_2    = v_dat[ particule_id_2 ];
           myVelocity_2.x += myAccel_2.x * 2.0f;
           myVelocity_2.y += myAccel_2.y * 2.0f;
           myVelocity_2.z += myAccel_2.z * 2.0f;
           myPosition_2.x  = myPosition_2.x + (myVelocity_2.x * 0.01f);
           myPosition_2.y  = myPosition_2.y + (myVelocity_2.y * 0.01f);
           myPosition_2.z  = myPosition_2.z + (myVelocity_2.z * 0.01f);

    threadgroup_barrier(mem_flags::mem_none);

  n_pos[ particule_id_2 ] = myPosition_2;     // NEXT POSITION !
  v_dat[ particule_id_2 ] = myVelocity_2;     // NEW  VELOCITY !
}
