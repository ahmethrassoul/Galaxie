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
#include <cstdio>
#include <stdbool.h>
#include <cmath>
#include <sys/time.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>

#include "./datatype/Loader.hpp"								// le loader !

#include "Backend/NullRender/NullRender.hpp"
#include "Backend/SDLBlackPixel/SDLBlackPixel.hpp"
#include "Backend/SDLColorPixel/SDLColorPixel.hpp"
#include "Backend/SpriteOpenGL/SpriteOpenGL.hpp"
#include "Backend/BackEndSprite/BackEndSprite.hpp"
#include "Backend/BackEndColoredSprite/BackEndColoredSprite.hpp"

#include "./kernel/Kernel.hpp"
#include "./kernel/null/RenderNull.hpp"
#include "./kernel/naive/RenderNaive.hpp"
#include "./kernel/optimized/RenderOptim.hpp"
#include "./kernel/optim_openmp/RenderOptimOpenMP.hpp"

#include "./kernel/simd/NEON/RenderNEON.hpp"
#include "./kernel/simd/SSE4/RenderSSE4.hpp"
#include "./kernel/simd/AVX2/RenderAVX2.hpp"
#include "./kernel/simd/AVX512/RenderAVX512.hpp"

#include "./kernel/openmp/RenderOpenMP.hpp"

#include "./kernel/simd_openmp/NEON/RenderNEONOpenMP.hpp"
#include "./kernel/simd_openmp/SSE4/RenderSSE4OpenMP.hpp"
#include "./kernel/simd_openmp/AVX2/RenderAVX2OpenMP.hpp"
#include "./kernel/simd_openmp/AVX512/RenderAVX512OpenMP.hpp"

#include "./kernel/metal/RenderMetalApple.hpp"
#include "./kernel/metal_v2/RenderMetalApple_v2.hpp"

#include "./kernel/cuda/RenderCUDA.hpp"

#include "./catch2v3/catch_amalgamated.hpp"

//
//
////////////////////////////////////////////////////////////////////
//
//

const float epsilon    = 0.01;
const int   iterations =   32;
const int   step       =   16;
//
//
////////////////////////////////////////////////////////////////////
//
//
#include "./tests/test_optimized.hpp"
//
//
////////////////////////////////////////////////////////////////////
//
//
#include "./tests/test_openmp.hpp"
//
//
////////////////////////////////////////////////////////////////////
//
//
#include "./tests/test_optim_openmp.hpp"
//
//
////////////////////////////////////////////////////////////////////
//
//
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    #include "./tests/test_neon.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    #include "./tests/test_neon_openmp.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#if defined(__APPLE__)
#include "./tests/test_macos_metal.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#if defined(__APPLE__)
    #include "./tests/test_macos_metal_v2.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#ifdef __SSE4_2__
    #include "./tests/test_sse4.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#ifdef __SSE4_2__
    #include "./tests/test_sse4_openmp.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#ifdef __AVX2__
    #include "./tests/test_avx2.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#ifdef __AVX2__
    #include "./tests/test_avx2_openmp.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#ifdef __AVX512F__
    #include "./tests/test_avx512.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#ifdef __AVX512F__
    #include "./tests/test_avx512_openmp.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
#if defined(_ENABLE_CUDA_)
    #include "./tests/test_cuda.hpp"
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
