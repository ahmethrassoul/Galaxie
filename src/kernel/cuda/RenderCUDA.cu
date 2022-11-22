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
#if defined(__NVCC__)
#include "RenderCUDA.hpp"
#include <iostream>


RenderCUDA::RenderCUDA( struct galaxy g ) : galaxie( g )
{
    nElements = galaxie.size;

    cudaError_t err = cudaSuccess;


    c_pos = (float4*)malloc( nElements * sizeof(float4) );
    if (c_pos == nullptr) {
        fprintf(stderr, "Failed to allocate host vector c_pos !\n");
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    n_pos = (float4*)malloc( nElements * sizeof(float4) );
    if (n_pos == nullptr) {
        fprintf(stderr, "Failed to allocate host vector n_pos !\n");
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    v_dat = (float4*)malloc( nElements * sizeof(float4) );
    if (v_dat == nullptr) {
        fprintf(stderr, "Failed to allocate host vector v_dat !\n");
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void **)&gpu_c_pos, nElements * sizeof(float4));
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate device vector gpu_c_pos (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void **)&gpu_n_pos, nElements * sizeof(float4));
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate device vector gpu_n_pos (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void **)&gpu_v_dat, nElements * sizeof(float4));
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate device vector gpu_v_dat (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    //

    for(int i = 0; i < nElements; i += 1)
    {
        float4 tmp;
        tmp.x    = g.pos_x[i];
        tmp.y    = g.pos_y[i];
        tmp.z    = g.pos_z[i];
        tmp.w    = g.mass [i];
        c_pos[i] = tmp;
    }

    err = cudaMemcpy(gpu_c_pos, c_pos, nElements * sizeof(float4), cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector c_pos from host to device (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    //

    for(int i = 0; i < nElements; i += 1)
    {
        float4 tmp;
        tmp.x        = 0.f;
        tmp.y        = 0.f;
        tmp.z        = 0.f;
        tmp.w        = 0.f;
        tmp.x    = g.pos_x[i];
        tmp.y    = g.pos_y[i];
        tmp.z    = g.pos_z[i];
        tmp.w    = g.mass [i];
        n_pos[i] = tmp;
    }

    err = cudaMemcpy(gpu_n_pos, n_pos, nElements * sizeof(float4), cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector c_pos from host to device (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    //

    for(int i = 0; i < nElements; i += 1)
    {
        float4 tmp;
        tmp.x        = g.vel_x[i];
        tmp.y        = g.vel_y[i];
        tmp.z        = g.vel_z[i];
        tmp.w        =        0.f;
        v_dat[i] = tmp;
    }

    err = cudaMemcpy(gpu_v_dat, v_dat, nElements * sizeof(float4), cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector v_dat from host to device (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

}
/*
 *
 *
 *##############################################################################
 *
 *
 */
RenderCUDA::~RenderCUDA()
{
    free( n_pos );
    free( c_pos );
    free( v_dat );

    cudaFree( gpu_c_pos );
    cudaFree( gpu_n_pos );
    cudaFree( gpu_v_dat );
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
extern __global__  void body2Body_exec(
                                        const float4* c_pos,
                                              float4* n_pos,
                                              float4* v_dat,
                                        const int     nElements);
//
extern __global__  void body2Body_copy(
                                              float4* c_pos,
                                        const float4* n_pos,
                                        const float4* v_dat,
                                        const int     nElements);
/*
 *
 *
 *##############################################################################
 *
 *
 */
void RenderCUDA::execute()
{
    //
    // Launch the Vector Add CUDA Kernel
    //
    cudaError_t err;
    int threadsPerBlock = 256; // max value as encoded in cuda description
    int blocksPerGrid = (nElements + threadsPerBlock - 1) / threadsPerBlock;

    body2Body_exec<<<blocksPerGrid, threadsPerBlock, threadsPerBlock * sizeof(float4)>>>(gpu_c_pos, gpu_n_pos, gpu_v_dat, nElements);

    err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch body2Body_exec kernel (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    body2Body_copy<<<blocksPerGrid, threadsPerBlock>>>(gpu_c_pos, gpu_n_pos, gpu_v_dat, nElements);

    err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch body2Body_copy kernel (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    //
    // Copy the device result vector in device memory to the host result vector
    // in host memory.
    //

    err = cudaMemcpy(n_pos, gpu_n_pos, nElements * sizeof(float4), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector gpu_c_pos from device to host (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    //
    // Copying particule positions to draw them in the GUI
    //

    for(int i = 0; i < nElements; i += 1)
    {
        float4 tmp = n_pos[i];
        galaxie.pos_x[i] = tmp.x;
        galaxie.pos_y[i] = tmp.y;
        galaxie.pos_z[i] = tmp.z;
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
Galaxy* RenderCUDA::particules()
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