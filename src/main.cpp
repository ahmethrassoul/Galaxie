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
#include "./kernel/auto_simd/RenderAutoSIMD.hpp"

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

#include "./kernel/cuda/RenderCUDA.hpp"

//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//

#include <getopt.h>

bool iequals(const std::string& a, const std::string& b)
{
    const int az = a.size();
    const int bz = b.size();
    if (az != bz)
        return false;
    for (int i = 0; i < az; i += 1)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

#include "./datatype/Loader.hpp"								// le loader !
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
int main( int argc, char ** argv )
{
#ifdef __AVX512BW__
    printf("(II) GalaxEirb student project (AVX512 version)\n");
#elif __AVX2__
    printf("(II) GalaxEirb student project (AVX2 version)\n");
#else
    printf("(II) GalaxEirb student project (ARM NEON version)\n");
#endif

    printf("(II) + developped by Bertrand LE GAL in 2022 ... 20xx\n");
    printf("(II) +        and by Jeremie CRENNE  in 2012 ... 2021\n");
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
//    std::time_t t = std::time(nullptr);
//    std::cout << "#(II) + Trace date and time : " << std::put_time(std::localtime(&t), "%c %Z") << '\n';
#endif
    printf("#(II)\n");

    int width  = 640 * 7 / 4;
    int height = 480 * 6 / 4;

    std::string filename = "../galaxies/dubinski_colored.tab";
    std::string colors   = "../data/colors/BlueRed.txt";

	int pas = 8;
    float delta_time = 0.01f;

    std::string impl = "";
    std::string view = "";

	while (1)
	{
        int option_index = 0;
        static struct option long_options[] = {
            {"impl",       required_argument,  0,  'I' },
            {"view",       required_argument,  0,  'V' },
            {"colors",     required_argument,  0,  'C' },
            {"width",      required_argument,  0,  'w' },
            {"height",     required_argument,  0,  'h' },
            {"delta-time", required_argument,  0,  'd' },
            {"verbose",    no_argument,        0,  'v' },
            {"step",       required_argument,  0,  's' },
            {"file",       required_argument,  0,  'f' },
            {0,                            0,  0,   0  }
        };
		
		int c = getopt_long(argc, argv, "I:V:w:h:d:vs:f:", long_options, &option_index);
        if (c == -1)
            break;

    	switch (c) {
            case 0:
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'I': impl       =       optarg;   break; // Select the optimized computation kernel
            case 'V': view       =       optarg;   break; // Select the visualization GUI
            case 'C': colors     =       optarg;   break; // Select a color space for particules
            case 'w': width      = atoi( optarg ); break; // set the width  of the GUI
            case 'h': height     = atoi( optarg ); break; // set the heigth of the GUI
            case 'd': delta_time = atof( optarg ); break;
            case 'f': filename   = optarg; break;

            case 's':
                pas = atoi( optarg );
                break;
       default:
            printf("?? getopt returned character code 0%o ??\n",     c);
            printf("?? getopt returned character code %d ??\n", (int)c);
            printf("?? getopt returned character code %c ??\n",      c);
        }
    }

	//
	// Chargement des données issues du fichier
	//

	galaxy gg = LoadFromFile( filename, pas );
	if( gg.size == 0 )
	{
        std::cout << "(EE) Error the galaxy has no paricule (g.size == " << gg.size << ")" << std::endl;
        return EXIT_FAILURE;
	}

	// Conversion du modele C => C++

//    float BIG_G = 6.67e-11; // gravitational constant

    Kernel* k = nullptr;
    if     ( iequals(impl, "null")          ) k = new RenderNull        ( gg );
    else if( iequals(impl, "naive")         ) k = new RenderNaive       ( gg );
    else if( iequals(impl, "optimized")     ) k = new RenderOptim       ( gg );
    else if( iequals(impl, "auto-simd")     ) k = new RenderAutoSIMD    ( gg );
    else if( iequals(impl, "openmp")        ) k = new RenderOpenMP      ( gg );

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    else if( iequals(impl, "neon")          ) k = new RenderNEON        ( gg );    // ARM processors
    else if( iequals(impl, "neon-openmp")   ) k = new RenderNEONOpenMP  ( gg );
#endif

#ifdef __SSE4_2__
    else if( iequals(impl, "sse4")          ) k = new RenderSSE4        ( gg );    // INTEL processors
    else if( iequals(impl, "sse4-openmp")   ) k = new RenderSSE4OpenMP  ( gg );
#endif

#ifdef __AVX2__
    else if( iequals(impl, "avx2")          ) k = new RenderAVX2        ( gg );    // INTEL processors
    else if( iequals(impl, "avx2-openmp")   ) k = new RenderAVX2OpenMP  ( gg );
#endif

#ifdef __AVX512F__
    else if( iequals(impl, "avx512")        ) k = new RenderAVX512      ( gg );    // INTEL processors
    else if( iequals(impl, "avx512-openmp") ) k = new RenderAVX512OpenMP( gg );
#endif

#if defined(__APPLE__)
    else if( iequals(impl, "metal")         ) k = new RenderMetalApple  ( gg );
#endif

#if defined(_ENABLE_CUDA_)
    else if( iequals(impl, "cuda")          ) k = new RenderCUDA        ( gg );
#endif

    if( k == nullptr)
    {
        std::cout << "(EE) The processing is undefined (type == " << impl << ")" << std::endl;
        std::cout << "(EE) Supported values for [--impl] are :" << std::endl;
        std::cout << "(EE) - null          : " << std::endl;
        std::cout << "(EE) - naive         : " << std::endl;
        std::cout << "(EE) - optimized     : " << std::endl;
        std::cout << "(EE) - auto-simd     : " << std::endl;
        std::cout << "(EE) - openmp        : " << std::endl;

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
        std::cout << "(EE) - neon          : " << std::endl;
        std::cout << "(EE) - neon-openmp   : " << std::endl;
#endif

#ifdef __SSE4_2__
        std::cout << "(EE) - sse4          : " << std::endl;
        std::cout << "(EE) - sse4-openmp   : " << std::endl;
#endif

#ifdef __AVX2__
        std::cout << "(EE) - avx2          : " << std::endl;
        std::cout << "(EE) - avx2-openmp   : " << std::endl;
#endif

#ifdef __AVX512F__
        std::cout << "(EE) - avx512        : " << std::endl;
        std::cout << "(EE) - avx512-openmp : " << std::endl;
#endif

#if defined(__APPLE__)
        std::cout << "(EE) - metal         : " << std::endl;
        return EXIT_FAILURE;
#endif

#if defined(_ENABLE_CUDA_)
        std::cout << "(EE) - cuda          : " << std::endl;
        return EXIT_FAILURE;
#endif
    }

    delete_galaxy( gg );

    printf("(II) BEFORE = %d x %d\n", width, height);
    k->set_dt( delta_time );
    printf("(II) AFTER  = %d x %d\n", width, height);

    printf("(II) Window dimensions = %d x %d\n", width, height);

	struct timeval begin, end;

	double tSum = 0.0;
	double nRun = 0.0;

    //
    // On cree la fenetre graphique
    //

    Render* backend = nullptr;

         if( iequals(view, "null")       ) backend = new NullRender          ( width, height, colors );
#if defined(_ENABLE_SDL_)
    else if( iequals(view, "BWPixel")    ) backend = new SDLBlackPixel       ( width, height, colors ); // SDL
    else if( iequals(view, "ColorPixel") ) backend = new SDLColorPixel       ( width, height, colors ); // SDL
    else if( iequals(view, "Sprite")     ) backend = new SpriteOpenGL        ( width, height, colors );
    else if( iequals(view, "OpenGL")     ) backend = new BackEndSprite       ( width, height, colors );
    else if( iequals(view, "CoolOpenGL") ) backend = new BackEndColoredSprite( width, height, colors );
#endif
    if( backend == nullptr)
    {
        std::cout << "(EE) The processing is undefined (type == " << view << ")" << std::endl;
        std::cout << "(EE) Supported values for [--view] are :" << std::endl;
        std::cout << "(EE) - Null       : No visualization => for performance measurement" << std::endl;
#if defined(_ENABLE_SDL_)
        std::cout << "(EE) - BWPixel    : SDL backend in black and white"                  << std::endl;
        std::cout << "(EE) - ColorPixel : SDL backend with colors"                         << std::endl;
        std::cout << "(EE) - Sprite     : SDL backend with sprites"                        << std::endl;
        std::cout << "(EE) - OpenGL     : OpenGL backend with sprites"                     << std::endl;
        std::cout << "(EE) - CoolOpenGL : OpenGL backend with COOL sprites"                << std::endl;
#endif
        return EXIT_FAILURE;
    }

	while ( backend->isFinished() == false )
	{

        //
        ////////////////////////////////////////////////////////////////////////////////
        //

        backend->refresh_interface();    // On re-initialise l'interface OpenGL

		gettimeofday( &begin, NULL );

        //
        ////////////////////////////////////////////////////////////////////////////////
        //
        backend->render( k->particules() );
//		k->render();    // On push les particules dans l'interface OpenGL
        k->execute();   // On calcule les nouvelles coordonnées des particules

		//
		////////////////////////////////////////////////////////////////////////////////
		//

		gettimeofday( &end, NULL );

		//
		////////////////////////////////////////////////////////////////////////////////
		//

		const float fps = (float)1.0f / ( ( end.tv_sec - begin.tv_sec ) * 1000000.0f + end.tv_usec - begin.tv_usec) * 1000000.0f;
		tSum += fps;
		nRun += 1.0;
		float avgFPS = tSum / nRun;

        backend->refresh_fps( avgFPS );

		//
		////////////////////////////////////////////////////////////////////////////////
		//
	}

    delete backend;

	//
	////////////////////////////////////////////////////////////////////////////////////
	//


	//
	////////////////////////////////////////////////////////////////////////////////////
	//

	return 1;
}

