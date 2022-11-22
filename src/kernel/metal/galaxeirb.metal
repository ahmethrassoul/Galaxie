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
 kernel void body2Body_copy(
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
    if( particule_id < nElements[0] )
    {
        c_pos[ particule_id ] = n_pos[ particule_id ];
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
kernel void body2Body_exec(
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

    const uint particule_id    = bid * blockDim + tid;
    const uint max_particules = nElements[0];

    //
    // Si jamais a cause des threads/grilles ont deborde...
    //

    if (particule_id >= max_particules)
    {
        return;
    }

    //
    // Dans le cas contraire on commence notre calcul !
    //

    float4 myPosition = c_pos[ particule_id ];
    // ... ... ... ... ... ... ...

    //
    //
    //

    float3 myAccel = {0.f, 0.f, 0.f};
    // ... ... ... ... ... ... ...

    for (uint idx = 0; idx < max_particules; idx += blockDim)
    {

        shPosition[tid] = c_pos[idx + tid];        // prefetching in local memory
        threadgroup_barrier(mem_flags::mem_none);  // prefetching in local memory

        for (uint p = 0; p < blockDim; p += 1)
        {
            const float4 other = shPosition[p];

            float rx = other.x - myPosition.x;
            float ry = other.y - myPosition.y;
            float rz = other.z - myPosition.z;

            float d = (rx * rx) + (ry * ry) + (rz * rz);
            // ... ... ... ... ... ... ...
#if 1
            float d3;
            if ( d < 1.0f ) d3 = (10.0f * other.w                    );
            else            d3 = (10.0f * other.w / (sqrt(d) * d));
#else
            float d3;
            if ( d < 1.0f )
            {
                d3 = 10.0f * other.w; // Multi modifiable
            }
            else
            {
                d  = sqrt(d);
                d3 = 10.0f * other.w / (d * d * d); // Multi modifiable
            }
#endif
            myAccel.x += rx * d3;
            myAccel.y += ry * d3;
            myAccel.z += rz * d3;
        }

        threadgroup_barrier(mem_flags::mem_none);

    }

    //
    // update position
    //

    threadgroup_barrier(mem_flags::mem_none);

    float4 myVelocity    = v_dat[ particule_id ];
           myVelocity.x += myAccel.x * 2.0f;
           myVelocity.y += myAccel.y * 2.0f;
           myVelocity.z += myAccel.z * 2.0f;
           myPosition.x  = myPosition.x + (myVelocity.x * 0.01f);
           myPosition.y  = myPosition.y + (myVelocity.y * 0.01f);
           myPosition.z  = myPosition.z + (myVelocity.z * 0.01f);

    threadgroup_barrier(mem_flags::mem_none);

    //
    // update device memory
    //
    n_pos[ particule_id ] = myPosition;     // NEXT POSITION !
    v_dat[ particule_id ] = myVelocity;     // NEW  VELOCITY !
}
