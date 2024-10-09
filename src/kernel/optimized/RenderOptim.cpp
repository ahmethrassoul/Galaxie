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
#include "RenderOptim.hpp"
#include <strings.h>
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderOptim::RenderOptim( struct galaxy g ) : galaxie( g )
{
    
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderOptim::RenderOptim( Galaxy& g ) : galaxie( g )
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void RenderOptim::execute()
{
    startExec();    // this is for fps computation


    //
    // On calcule les nouvelles positions de toutes les particules
    //

    for(int i = 0; i < galaxie.size; i += 1)
    {
        float pos_x_i = galaxie.pos_x[i];
        float pos_y_i = galaxie.pos_y[i];
        float pos_z_i = galaxie.pos_z[i];

        float accel_x = 0.f;
        float accel_y = 0.f;
        float accel_z = 0.f;

        // simd

        for(int j = 0; j < galaxie.size; j += 1)
        {
            if( i != j )
            {
                const float dx = galaxie.pos_x[j] - pos_x_i;
                const float dy = galaxie.pos_y[j] - pos_y_i;
                const float dz = galaxie.pos_z[j] - pos_z_i;

                const float dij = dx * dx + dy * dy + dz * dz;

                float d3;
                const float m_a = 10.0f * galaxie.mass[j];
                if ( dij < 1.0f )
                {
                    d3 = m_a; // Multi modifiable
                }
                else
                {
                    const float sqrtd = sqrtf(dij);
                    d3 = m_a / (sqrtd * dij); // Multi modifiable
                }

                accel_x += (dx * d3);
                accel_y += (dy * d3);
                accel_z += (dz * d3);
            }
        }

        //////simd end

        galaxie.vel_x[i] += (accel_x * 2);
        galaxie.vel_y[i] += (accel_y * 2);
        galaxie.vel_z[i] += (accel_z * 2);
    }
    
    for(int i = 0; i < galaxie.size; i += 1)
    {
        galaxie.pos_x[i] += (galaxie.vel_x[i] * dt);
        galaxie.pos_y[i] += (galaxie.vel_y[i] * dt);
        galaxie.pos_z[i] += (galaxie.vel_z[i] * dt);
    }

    stopExec();    // this is for fps computation
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderOptim::~RenderOptim()
{
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy* RenderOptim::particules()
{
    return &galaxie;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
