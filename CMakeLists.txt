cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

#
# To be enable for target with NVIDIA GPUs
#
#project(cmake_and_cuda LANGUAGES CXX CUDA)
#enable_language(CUDA)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
message("${CMAKE_SOURCE_DIR}/cmake/Modules/")


###################################################################################################
#
#  APPLE MACOS (ARM M1/M2 cores)
#

#SET(CMAKE_CXX_STANDARD    17)    # 17 choisi pour metal
#SET(CMAKE_BUILD_TYPE Release)

#SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/15.0.3/bin/clang)
#SET (CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.3/bin/clang++)

#SET(CMAKE_CXX_FLAGS "-Wall")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=native -mtune=native -std=c++17")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_SDL_")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/include") # MACOS ARM (BREW)

#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib")    # MACOS ARM (BREW)
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")      # OPENGL
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal")       # METAL
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Foundation ") # METAL
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework MetalKit")    # METAL


####################################################################################################
#
#  APPLE MACOS (INTEL x86 cores)
#

#SET(CMAKE_CXX_STANDARD    17)    # 17 choisi pour metal
#SET(CMAKE_BUILD_TYPE Release)

#SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/15.0.3/bin/clang)
#SET (CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.3/bin/clang++)

#SET(CMAKE_CXX_FLAGS "-Wall")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -std=c++17")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_SDL_")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")                 # MACOS INTEL (BREW)

#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib")       # MACOS INTEL (BREW)
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")      # OPENGL
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal")       # METAL
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Foundation ") # METAL
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework MetalKit")    # METAL


###################################################################################################
#
#  LINUX @T ENSEIRB-MATMECA (INTEL x86 cores)
#

SET(CMAKE_CXX_STANDARD    11)
SET(CMAKE_BUILD_TYPE Release)

##SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/15.0.3/bin/clang)
##SET (CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.3/bin/clang++)

SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_SDL_")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")                 # MACOS INTEL (BREW)


###################################################################################################
#
#  LINUX on NVIDIA ORIN devices (ARM + GPU cores)
#

#SET(CMAKE_CXX_STANDARD    17)
#SET(CMAKE_BUILD_TYPE Release)

#SET(CMAKE_CXX_FLAGS "-Wall")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -std=c++17")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_SDL_")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_CUDA_")

###################################################################################################
#
#  LINUX on RASPBERRY PI-4 devices (ARM)
#

#SET(CMAKE_CXX_STANDARD    17)
#SET(CMAKE_BUILD_TYPE Release)

#SET(CMAKE_CXX_FLAGS "-Wall")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -std=c++17")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ENABLE_SDL_")

########################################################################################################################
#
project(galax_eirb)

# #
 set(SOURCE_FILES
         ./src/datatype/datatype.cpp
         ./src/datatype/Loader.cpp
         ./src/Galaxy/Galaxy.cpp
         ./src/kernel/Kernel.cpp
         ./src/kernel/null/RenderNull.cpp                       # NULL RENDER
         ./src/kernel/naive/RenderNaive.cpp
         ./src/kernel/optimized/RenderOptim.cpp
         ./src/kernel/auto_simd/RenderAutoSIMD.cpp              # AUTO-SIMD ?
         ./src/kernel/simd/NEON/RenderNEON.cpp                  # SIMD NEON
         ./src/kernel/simd/SSE4/RenderSSE4.cpp                  # SIMD SSE4
         ./src/kernel/simd/AVX2/RenderAVX2.cpp                  # SIMD AVX2
         ./src/kernel/simd/AVX512/RenderAVX512.cpp              # SIMD AVX512
         ./src/kernel/metal/RenderMetalApple.cpp                # APPLE METAL (GPU)
         ./src/kernel/cuda/RenderCUDA.cu                        # NVIDIA CUDA (GPU)
         ./src/kernel/cuda/galaxeirb.cu                         # NVIDIA CUDA (GPU)
         ./src/kernel/openmp/RenderOpenMP.cpp                   # OPENMP
         ./src/kernel/simd_openmp/NEON/RenderNEONOpenMP.cpp     # OPENMP + NEON
         ./src/kernel/simd_openmp/SSE4/RenderSSE4OpenMP.cpp     # OPËNMP + SSE4
         ./src/kernel/simd_openmp/AVX2/RenderAVX2OpenMP.cpp     # OPENMP + AVX2
         ./src/kernel/simd_openmp/AVX512/RenderAVX512OpenMP.cpp # OPENMP + AVX512

         ./src/Backend/Render.cpp                               # Generic window classes
         ./src/Backend/SDLRender/SDLRender.cpp                  # Generic window classes
         ./src/Backend/OpenGLRender/OpenGLRender.cpp            # Generic window classes

         ./src//Backend/NullRender/NullRender.cpp
         ./src//Backend/OnlyOpenGL/OnlyOpenGL.cpp
         ./src//Backend/SDLBlackPixel/SDLBlackPixel.cpp
         ./src//Backend/SDLColorPixel/SDLColorPixel.cpp
         ./src//Backend/SpriteOpenGL/SpriteOpenGL.cpp
         ./src//Backend/BackEndSprite/tools.cpp
         ./src//Backend/BackEndSprite/BackEndSprite.cpp
         ./src//Backend/BackEndColoredSprite/BackEndColoredSprite.cpp
         ./src/Backend/RGBColors/RGBColors.cpp
         ./src/Backend/others/text.cpp
         ./src/main.cpp )


add_executable(galax_eirb ${SOURCE_FILES})

target_link_libraries(galax_eirb SDL2)
target_link_libraries(galax_eirb GLEW)
target_link_libraries(galax_eirb  GLU)
target_link_libraries(galax_eirb   GL)
