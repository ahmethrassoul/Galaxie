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
#if defined(__APPLE__)
#include "RenderMetalApple_v2.hpp"
#include <iostream>
#include <cmath>

RenderMetalApple_v2::RenderMetalApple_v2( struct galaxy g ) : galaxie( g )
{

    nElements = galaxie.size; // le nombre d'elements dans la galaxie

    _mDevice = MTL::CreateSystemDefaultDevice();
    
    if( (_mDevice->lowPower() == true) && (_mDevice->removable() == false) )
    {
        sharedMemory = true;
    }
    else if( (_mDevice->lowPower() == false) && (_mDevice->removable() == false) )
    {
        sharedMemory = false;
    }
    else
    {
        std::cout << "(EE) The GPU device is an external one !" << std::endl;
        exit( EXIT_FAILURE );
    }

    NS::Error *error = nullptr;

    //
    // Load the shader files with a .metal file extension in the project
    //
#if 1
    NS::String* filePath         = NS::String::string("./galaxeirb_v2.metallib", NS::UTF8StringEncoding);
    MTL::Library *defaultLibrary = _mDevice->newLibrary(filePath, &error);
#else
    MTL::Library *defaultLibrary = _mDevice->newDefaultLibrary();
#endif
    if (defaultLibrary == nullptr)
    {
        std::cout << "(EE) Failed to find the default library." << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    //
    //
    auto str1 = NS::String::string("body2Body_exec_v2", NS::ASCIIStringEncoding);
    MTL::Function *execFunction = defaultLibrary->newFunction(str1);
    if (execFunction == nullptr)
    {
        std::cout << "(EE) Failed to find the (body2Body_exec_v2) function." << std::endl;
        exit( EXIT_FAILURE );
    }

    auto str2 = NS::String::string("body2Body_copy_v2", NS::ASCIIStringEncoding);
    MTL::Function *copyFunction = defaultLibrary->newFunction(str2);
    if (copyFunction == nullptr)
    {
        std::cout << "(EE) Failed to find the (body2Body_copy_v2) function." << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    // Create a compute pipeline state object.
    //
    _mExecFunctionPSO = _mDevice->newComputePipelineState(execFunction, &error);
    if (_mExecFunctionPSO == nullptr)
    {
        std::cout << "Failed to created pipeline state object, error " << error << "." << std::endl;
        exit( EXIT_FAILURE );
    }

    _mCopyFunctionPSO = _mDevice->newComputePipelineState(copyFunction, &error);
    if (_mCopyFunctionPSO == nullptr)
    {
        std::cout << "Failed to created pipeline state object, error " << error << "." << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    //
    //
    _mCommandQueue = _mDevice->newCommandQueue();
    if (_mCommandQueue == nullptr)
    {
        std::cout << "Failed to find the command queue." << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    //  Allocation des zones mémoire permettant d'échanger avec le GPU
    //

    c_pos = _mDevice->newBuffer(sizeof(simd::float4) * nElements, MTL::StorageModeManaged); // x, y, z, m
    n_pos = _mDevice->newBuffer(sizeof(simd::float4) * nElements, MTL::StorageModeManaged); // x, y, z, 0
    v_dat = _mDevice->newBuffer(sizeof(simd::float4) * nElements, MTL::StorageModeManaged); // vx, vy, vz, 0
    size  = _mDevice->newBuffer(sizeof(uint32_t),                 MTL::StorageModeManaged); // vx, vy, vz, 0

    //

    uint32_t* nE = (uint32_t*)size->contents();
    nE[0]        = nElements;

    //

    simd::float4* ptr_c_pos = (simd::float4*)c_pos->contents();
    for(int i = 0; i < nElements; i += 1)
    {
        simd::float4 tmp;
        tmp.x        = g.pos_x[i];
        tmp.y        = g.pos_y[i];
        tmp.z        = g.pos_z[i];
        tmp.w        = g.mass [i];
        ptr_c_pos[i] = tmp;
    }

    //

    simd::float4* ptr_n_pos = (simd::float4*)n_pos->contents();
    for(int i = 0; i < nElements; i += 1)
    {
        simd::float4 tmp;
        tmp.x        = 0.f;
        tmp.y        = 0.f;
        tmp.z        = 0.f;
        tmp.w        = 0.f;
        ptr_n_pos[i] = tmp;
    }

    //

    simd::float4* ptr_v_dat = (simd::float4*)v_dat->contents();
    for(int i = 0; i < nElements; i += 1)
    {
        simd::float4 tmp;
        tmp.x        = g.vel_x[i];
        tmp.y        = g.vel_y[i];
        tmp.z        = g.vel_z[i];
        tmp.w        =        0.f;
        ptr_v_dat[i] = tmp;
    }

    //
    // When GPU does not share CPU memory the values should be transfered
    //

    if( sharedMemory == false )
    {
        NS::Range rangE(0,     sizeof(uint32_t)         );
        NS::Range range(0, 4 * sizeof(float) * nElements);
        size ->didModifyRange( rangE );
        c_pos->didModifyRange( range );
        n_pos->didModifyRange( range );
        v_dat->didModifyRange( range );
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
RenderMetalApple_v2::~RenderMetalApple_v2()
{

}
/*
 *
 *
 *##############################################################################
 *
 *
 */
void RenderMetalApple_v2::execute()
{

}
/*
 *
 *
 *##############################################################################
 *
 *
 */
int RenderMetalApple_v2::N()
{
    return nElements;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
Galaxy* RenderMetalApple_v2::particules()
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