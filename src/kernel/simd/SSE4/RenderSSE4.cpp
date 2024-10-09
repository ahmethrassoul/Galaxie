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
