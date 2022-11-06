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
#if !defined(_RenderCUDA_) && ( defined(__NVCC__) || defined(_ENABLE_CUDA_) )
#define _RenderCUDA_

#include "../Kernel.hpp"
#include <iostream>
#include <cstring>
#include <chrono>

#ifndef __NVCC__
    typedef struct float4 {
        float x;
        float y;
        float z;
        float w;
    } float4;
#endif

class RenderCUDA  : public Kernel
{
public:
    float4* c_pos;
    float4* n_pos;
    float4* v_dat;

    float4* gpu_c_pos;
    float4* gpu_n_pos;
    float4* gpu_v_dat;

    int nElements;

public:
     RenderCUDA(struct galaxy g );
    ~RenderCUDA();

    void execute();
    void render();
    Galaxy* particules();

private:
    Galaxy galaxie;
};
#endif