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
    const int simd  = sizeof(__m128) / sizeof(float);
/*
    __m128 buff_pos_x   = _mm_loadu_ps( galaxie.pos_x );
    __m128 buff_pos_y   = _mm_loadu_ps( galaxie.pos_y );
    __m128 buff_pos_z   = _mm_loadu_ps( galaxie.pos_z );
*/

    const __m128 tab_1      = _mm_set_ps1(1.0f);
    const __m128 tab_2      = _mm_set_ps1(2.0f);
    const __m128 tab_10     = _mm_set_ps1(10.0f);

    const __m128 buff_dij   = _mm_set_ps1(1.0f);

    const __m128 buff_mass  = _mm_loadu_ps( galaxie.mass );
    float buff_mass_tab[galaxie.size]; 
    _mm_storeu_ps(buff_mass_tab , buff_mass );

    for(int i = 0; i < galaxie.size; i += 1)
    {
        
        //__m128 buff_accel_x = _mm_zero_ps( galaxie.accel_x );
        //__m128 buff_accel_y = _mm_zero_ps( galaxie.accel_y );
        //__m128 buff_accel_z = _mm_zero_ps( galaxie.accel_z );

        const __m128 pos_x_i    = _mm_set_ps1( galaxie.pos_x[i] );
        const __m128 pos_y_i    = _mm_set_ps1( galaxie.pos_y[i] );
        const __m128 pos_z_i    = _mm_set_ps1( galaxie.pos_z[i] );



        float accel_x   = 0.f;
        float accel_y   = 0.f;
        float accel_z   = 0.f;

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

            const __m128 dij_temp       = _mm_add_ps       ( dx_2 , dy_2 );
            const __m128 dij            = _mm_add_ps       ( dij_temp , dz_2 );

            
            const __m128 ma_j_vec       = _mm_set_ps       ( galaxie.mass[j],galaxie.mass[j+1],galaxie.mass[j+2],galaxie.mass[j+3]);
            const __m128 m_a_simple     = _mm_mul_ps       ( ma_j_vec, tab_10 ); // vecteur de m_a
            const __m128 sqrt_dij       = _mm_sqrt_ps      ( dij ); 
            const __m128 dij_3          = _mm_mul_ps       ( sqrt_dij , dij );
            const __m128 m_a_complex    = _mm_div_ps       ( m_a_simple , dij_3 );


            // comparaison du if dij < 1.0f
            const __m128 masq           = _mm_cmplt_ps    ( dij , tab_1 );
            const __m128 d3             = _mm_blendv_ps   ( m_a_complex , m_a_simple  , masq );



        }

        // fin SIMD

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
//
//
