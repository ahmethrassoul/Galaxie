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
#include "RenderAVX2.hpp"
#ifdef __AVX2__
#include <immintrin.h>
#endif
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderAVX2::RenderAVX2( struct galaxy g ) : galaxie( g )
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderAVX2::RenderAVX2( Galaxy& g ) : galaxie( g )
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void RenderAVX2::execute()
{
#ifdef __AVX2__
    startExec();    // this is for fps computation

    stopExec();
#else
    printf("(EE) The current platform does not support INTEL AVX2 SIMD extension...\n");
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
RenderAVX2::~RenderAVX2()
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy* RenderAVX2::particules()
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
