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
    accel_x = new float[g.size];
    accel_y = new float[g.size];
    accel_z = new float[g.size];
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
    accel_x = new float[g.size];
    accel_y = new float[g.size];
    accel_z = new float[g.size];
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

    bzero(accel_x, sizeof(float) * galaxie.size);
    bzero(accel_y, sizeof(float) * galaxie.size);
    bzero(accel_z, sizeof(float) * galaxie.size);

    //
    // On calcule les nouvelles positions de toutes les particules
    //
    float accel_x = ;
    accel_y = ;
    accel_z = ;
    for(int i = 0; i < galaxie.size; i += 1)
    {
        const float pos_x_i = galaxie.pos_x[i];
        const float pos_y_i = galaxie.pos_y[i];
        const float pos_z_i = galaxie.pos_z[i];
        for(int j = 0; j < galaxie.size; j += 1)
        {
            if( i != j )
            {
                const float dx = galaxie.pos_x[j] - pos_x_i;
                const float dy = galaxie.pos_y[j] - pos_y_i;
                const float dz = galaxie.pos_z[j] - pos_z_i;

                float dij = dx * dx + dy * dy + dz * dz;

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

                accel_x[i] += (dx * d3);
                accel_y[i] += (dy * d3);
                accel_z[i] += (dz * d3);
            }
        }

    }

    accel_x = ;
    accel_y = ;
    accel_z = ;


    for(int i = 0; i < galaxie.size; i += 1)
    {
        galaxie.vel_x[i] += (accel_x[i] * 2.0f);
        galaxie.vel_y[i] += (accel_y[i] * 2.0f);
        galaxie.vel_z[i] += (accel_z[i] * 2.0f);

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
    delete[] accel_x;
    delete[] accel_y;
    delete[] accel_z;
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
