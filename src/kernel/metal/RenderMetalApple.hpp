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
#if !defined(_RenderMetalApple_) && defined(__APPLE__)
#define _RenderMetalApple_

#include "../Kernel.hpp"
#include <iostream>
#include <omp.h>
#include <cstring>
#include <chrono>
#include <random>

#include <simd/simd.h>
#include "./Metal.hpp"

 union
{
    float data[4];
    struct
    {
        float x;
        float y;
        float z;
        float m;
     };
 } truc;

class RenderMetalApple  : public Kernel
{
public:
    MTL::Device* _mDevice;

    // The compute pipeline generated from the compute kernel in the .metal shader file.
    MTL::ComputePipelineState *_mExecFunctionPSO;
    MTL::ComputePipelineState *_mCopyFunctionPSO;

    // The command queue used to pass commands to the device.
    MTL::CommandQueue *_mCommandQueue;

    // Buffers to hold data.
    MTL::Buffer* c_pos;
    MTL::Buffer* n_pos;
    MTL::Buffer* v_dat;
    MTL::Buffer*  size;

    int nElements;

    bool sharedMemory;

public:
     RenderMetalApple(struct galaxy g );
    ~RenderMetalApple();

    void execute();
    void render();
    Galaxy* particules();

private:
    Galaxy galaxie;

    float* ptr_A();
    float* ptr_B();
    float* ptr_C();
    int    N();

    void load_A (const float* src);
    void load_B (const float* src);
    void store_C(      float* dst);

private:
    void encodeAddCommand(MTL::ComputeCommandEncoder *computeEncoder);
    void generateRandomFloatData(MTL::Buffer *buffer);
};
#endif