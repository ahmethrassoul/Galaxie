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
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include "RenderNEONOpenMP.hpp"
#include "arm_neon.h"
#include <omp.h>
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderNEONOpenMP::RenderNEONOpenMP( struct galaxy g ) : galaxie( g )
{
    const int max_threads = omp_get_num_procs(); // no division by two there is no logical cores in ARM systems
    omp_set_num_threads(max_threads);
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderNEONOpenMP::RenderNEONOpenMP( Galaxy& g ) : galaxie( g )
{
    const int max_threads = omp_get_num_procs(); // no division by two there is no logical cores in ARM systems
    omp_set_num_threads(max_threads);
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void RenderNEONOpenMP::execute()
{
    startExec();    // this is for fps computation

    stopExec();    // this is for fps computation
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RenderNEONOpenMP::~RenderNEONOpenMP()
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy* RenderNEONOpenMP::particules()
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
#endif
