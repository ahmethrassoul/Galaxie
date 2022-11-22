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
#include <getopt.h>

#include "./datatype/Loader.hpp"								// le loader !

#include "./kernel/Kernel.hpp"

#include "./kernel/naive/RenderNaive.hpp"
#include "./kernel/optimized/RenderOptim.hpp"

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

//
//
////////////////////////////////////////////////////////////////////
//
//

int   batch_min  =    8;
int   time_value =   60;
int   lib_step   =    1;

int main( int argc, char ** argv )
{
#ifdef __AVX512BW__
    printf("(II) GalaxEirb benchmarking tool (AVX512 version)\n");
#elif __AVX2__
    printf("(II) GalaxEirb benchmarking tool (AVX2 version)\n");
#else
    printf("(II) GalaxEirb benchmarking tool (ARM NEON version)\n");
#endif

    printf("(II) + developped by Bertrand LE GAL in 2022 ... 20xx\n");
    printf("(II)\n");
    printf("(II) Binary generated : %s - %s\n", __DATE__, __TIME__);

#if defined(__clang__)
    /* Clang/LLVM. ---------------------------------------------- */
    printf("(II) + Clang/LLVM version %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__ICC) || defined(__INTEL_COMPILER)
    /* Intel ICC/ICPC. ------------------------------------------ */
    printf("(II) + Intel ICC/ICPC version %d.%d\n", __INTEL_COMPILER, __INTEL_COMPILER_BUILD_DATE);
#elif defined(__GNUC__) || defined(__GNUG__)
	/* GNU GCC/G++. --------------------------------------------- */
    printf("(II) + GNU GCC/G++ version %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
	/* Microsoft Visual Studio. --------------------------------- */
    printf("(II) + Microsoft Visual Studio\n");
#else
    #error "(II) + Undetected compiler !"
#endif

#if (defined(__ICC) || defined(__INTEL_COMPILER)) == 0
    std::time_t t = std::time(nullptr);
    std::cout << "#(II) + Trace date and time : " << std::put_time(std::localtime(&t), "%c %Z") << '\n';
#endif
    printf("(II)\n");

    std::string filename = "../galaxies/dubinski_colored.tab";

    while (1)
    {
        int option_index = 0;
        static struct option long_options[] = {
                {"verbose",    no_argument,        0,  'v' },
                {"step",       required_argument,  0,  's' },
                {"time",       required_argument,  0,  't' },
                {"file",       required_argument,  0,  'f' },
                {0,                            0,  0,   0  }
        };

        int c = getopt_long(argc, argv, "vs:t:f:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'f': filename   = optarg;
                break;

            case 's':
                lib_step = atoi( optarg );
                break;

            case 't':
                time_value = atoi( optarg );
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n",     c);
                printf("?? getopt returned character code %d ??\n", (int)c);
                printf("?? getopt returned character code %c ??\n",      c);
        }
    }

    //
    //
    //

    galaxy galaxie = LoadFromFile( filename, lib_step );
    if( galaxie.size == 0 )
    {
        std::cout << "(EE) Error the galaxy has no paricule (g.size == " << galaxie.size << ")" << std::endl;
        exit( EXIT_FAILURE );
    }

    Kernel* k_ref = new RenderNaive( galaxie );

    printf("(II) Launching RenderNaive measure\r");
    fflush(stdout);
    int n_ref = 0;
    auto start_ref = std::chrono::steady_clock::now();
    auto end_ref   = start_ref;
    do{
        for(int i = 0; i < 2; i += 1)
        {
            k_ref->execute();
        }
        end_ref = std::chrono::steady_clock::now();
        n_ref  += 2;//batch_min;
    }while(std::chrono::duration_cast<std::chrono::seconds>(end_ref - start_ref).count() < time_value);

    delete k_ref;

    auto execTime_ref = std::chrono::duration_cast<std::chrono::milliseconds>(end_ref - start_ref).count();
    const float fps_ref        =    (float)n_ref * (1000.0f / execTime_ref);


    {
        printf("(II) Launching RenderOptim measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderOptim( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < 2; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += 2;//batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod =    (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[optimized]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }


    {
        Kernel* k_mod = new RenderOpenMP( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod =    (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[openmp   ]  | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", (int)(execTime_mod/1000), fps_mod, speedup);
    }


#ifdef __SSE4_2__
    {
        printf("(II) Launching RenderSSE4 measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderSSE4( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[sse4     ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#ifdef __AVX2__
    {
        printf("(II) Launching RenderAVX2 measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderAVX2( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[avx2     ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#ifdef __AVX512F__
    {
        printf("(II) Launching RenderAVX512 measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderAVX512( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[avx512   ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    {
        printf("(II) Launching RenderNEON measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderNEON( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[neon     ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#ifdef __SSE4_2__
    {
        printf("(II) Launching RenderSSE4OpenMP measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderSSE4OpenMP( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[sse4-omp ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#ifdef __AVX2__
    {
        printf("(II) Launching RenderAVX2OpenMP measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderAVX2OpenMP( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[avx2-omp ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#ifdef __AVX512F__
    {
        printf("(II) Launching RenderAVX512OpenMP measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderAVX512OpenMP( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[avx512-om]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    {
        printf("(II) Launching RenderNEONOpenMP measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderNEONOpenMP( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[neon-omp ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#if defined(__APPLE__)
    {
        printf("(II) Launching RenderMetalApple measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderMetalApple( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[metal-mac]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#if defined(__APPLE__)
    {
        printf("(II) Launching RenderMetalApple_v2 measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderMetalApple_v2( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[metal_v2 ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif


#if defined(_ENABLE_CUDA_)
    {
        printf("(II) Launching RenderCUDA measure\r");
        fflush(stdout);
        Kernel* k_mod = new RenderCUDA( galaxie );

        int n_mod = 0;
        auto start_mod = std::chrono::steady_clock::now();
        auto end_mod   = start_mod;
        do{
            for(int i = 0; i < batch_min; i += 1)
            {
                k_mod->execute();
            }
            end_mod = std::chrono::steady_clock::now();
            n_mod  += batch_min;
        }while(std::chrono::duration_cast<std::chrono::seconds>(end_mod - start_mod).count() < time_value);

        auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();
        delete k_mod;

        const float fps_mod = (float)n_mod * (1000.0f / execTime_mod);
        const float speedup = fps_mod / fps_ref;

        printf("[nv-cuda  ]  | EXEC %4d | TIME %3ds | FPS %7.3f | Speed-up %7.3f |\n", n_mod, (int)(execTime_mod/1000), fps_mod, speedup);
    }
#endif

    return EXIT_SUCCESS;
}
