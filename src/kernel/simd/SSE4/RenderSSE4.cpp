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
#include "RenderSSE4.hpp"
#include <strings.h>
#ifdef __SSE4_2__
#include <immintrin.h>
#endif
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderSSE4::RenderSSE4( struct galaxy g ) : galaxie( g )
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderSSE4::RenderSSE4( Galaxy& g ) : galaxie( g )
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void RenderSSE4::execute()
{
#ifdef __SSE4_2__
    startExec();    // this is for fps computation
    const int simd  = sizeof(__m128) / sizeof(float);

    const __m128 tab_1      = _mm_set_ps1(1.0f);  
    const __m128 tab_10     = _mm_set_ps1(10.0f);


    for(int i = 0; i < galaxie.size; i += 1)
    {

        const __m128 pos_x_i    = _mm_set_ps1( galaxie.pos_x[i] );
        const __m128 pos_y_i    = _mm_set_ps1( galaxie.pos_y[i] );
        const __m128 pos_z_i    = _mm_set_ps1( galaxie.pos_z[i] );

        __m128 buff_accel_x     = _mm_setzero_ps();
        __m128 buff_accel_y     = _mm_setzero_ps();      
        __m128 buff_accel_z     = _mm_setzero_ps();

        // debut SIMD
        for(int j = 0; j < galaxie.size; j += simd)
        {
                  
            const __m128 load_dx        = _mm_loadu_ps     ( galaxie.pos_x + j );
            const __m128 dx             = _mm_sub_ps       ( load_dx , pos_x_i );
            const __m128 dx_2           = _mm_mul_ps       ( dx , dx );

            const __m128 load_dy        = _mm_loadu_ps     ( galaxie.pos_y + j );
            const __m128 dy             = _mm_sub_ps       ( load_dy , pos_y_i );
            const __m128 dy_2           = _mm_mul_ps       ( dy , dy );

            const __m128 load_dz        = _mm_loadu_ps     ( galaxie.pos_z + j );
            const __m128 dz             = _mm_sub_ps       ( load_dz , pos_z_i );
            const __m128 dz_2           = _mm_mul_ps       ( dz , dz );

            const __m128 dijp           = dx_2 + dy_2 + dz_2;
            const __m128 dij            = _mm_max_ps( dijp, tab_1); //maixmum avec 1
            
            const __m128 ma_j_vec       = _mm_loadu_ps     ( galaxie.mass + j );

            const __m128 m_a_simple     = ma_j_vec * tab_10; // vecteur de m_a = 10*glaxie.mass[j]
            const __m128 sqrt_dij       = _mm_sqrt_ps      ( dij );
            const __m128 dij_3          = _mm_mul_ps       ( sqrt_dij , dij );
            const __m128 m_a_complex    = _mm_div_ps       ( m_a_simple , dij_3 );

            const __m128 d3 = m_a_complex;

            buff_accel_x                    += _mm_mul_ps( d3 , dx);
            buff_accel_y                    += _mm_mul_ps( d3 , dy);
            buff_accel_z                    += _mm_mul_ps( d3 , dz);

        }

        float tab_accel_x[4], tab_accel_y[4], tab_accel_z[4];
        _mm_storeu_ps( tab_accel_x , buff_accel_x );
        _mm_storeu_ps( tab_accel_y , buff_accel_y );
        _mm_storeu_ps( tab_accel_z , buff_accel_z );

        const float accel_x = tab_accel_x[0] + tab_accel_x[1] + tab_accel_x[2] + tab_accel_x[3];
        const float accel_y = tab_accel_y[0] + tab_accel_y[1] + tab_accel_y[2] + tab_accel_y[3];
        const float accel_z = tab_accel_z[0] + tab_accel_z[1] + tab_accel_z[2] + tab_accel_z[3];
        
        galaxie.vel_x[i] += (accel_x * 2.0f);
        galaxie.vel_y[i] += (accel_y * 2.0f);
        galaxie.vel_z[i] += (accel_z * 2.0f);
    }

    for(int i = 0; i < galaxie.size; i += 1)
    {
        galaxie.pos_x[i] += (galaxie.vel_x[i] * dt);
        galaxie.pos_y[i] += (galaxie.vel_y[i] * dt);
        galaxie.pos_z[i] += (galaxie.vel_z[i] * dt);
    }

    stopExec();    // this is for fps computation
#else
    printf("(EE) The current platform does not support INTEL SSE4 SIMD extension...\n");
    exit( EXIT_FAILURE );
#endif
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderSSE4::~RenderSSE4()
{
}

//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy* RenderSSE4::particules()
{
    return &galaxie;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
// make && ./test "NAIVE vs OPTIMIZED"
// make && ./test "NAIVE vs INTEL SSE4"
//  ./test --list-tests
// ./galax_eirb --impl optimized --view null
//myate@barn-e-02:~/SNUM3/Galaxie/SE301-GalaxEirb-Student/src/kernel/simd/SSE4$