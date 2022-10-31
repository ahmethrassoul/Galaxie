cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
message("${CMAKE_SOURCE_DIR}/cmake/Modules/")

#
#  RASPBERRY PI 4 FLAGS
#

SET(CMAKE_CXX_STANDARD 17)    # 17 choisi pour metal
SET(CMAKE_BUILD_TYPE Release)

#SET (CMAKE_C_COMPILER   /usr/bin/clang)
#SET (CMAKE_CXX_COMPILER /usr/bin/clang++)
#SET (CMAKE_C_COMPILER   /opt/homebrew/bin/gcc-11)
#SET (CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-11)
#SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/13.0.1_1/bin/clang++

SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/15.0.2//bin/clang)
SET (CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.2/bin/clang++)

SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=native -mtune=native -std=c++17")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize") #

#
# LINKER FLAGS (CXX) SPECIFIQUES A MACOS (APPLE M1)
#

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/include")

# FLAGS (LINKER) SPECIFIQUES A MACOS

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib")   # UHD
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL") # UHD
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal -framework Foundation -framework MetalKit") # UHD

#
# LINKER FLAGS (CXX) SPECIFIQUES A MACOS (APPLE INTEL)
#

#SET(CMAKE_CXX_FLAGS        "${CMAKE_CXX_FLAGS}        -I/usr/local/include -D_MKL_")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib")   # UHD
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Accelerate") # UHD

########################################################################################################################
project(galax_eirb)

# #
 set(SOURCE_FILES
         ./src/others/geometry.cpp
         ./src/datatype/datatype.cpp
         ./src/data/Loader.cpp
         src/RGBColors/RGBColors.cpp
         ./src/Galaxy/Galaxy.cpp
         ./src/kernel/Kernel.cpp
         ./src/kernel/null/RenderNull.cpp
         ./src/kernel/naive/RenderNaive.cpp
         ./src/kernel/optimized/RenderOptim.cpp
         ./src/kernel/auto_simd/RenderAutoSIMD.cpp
         ./src/kernel/simd/RenderSIMD.cpp
         ./src/kernel/metal/RenderMetalApple.cpp
         ./src/kernel/openmp/RenderOpenMP.cpp
         ./src/kernel/simd_openmp/RenderSIMDOpenMP.cpp
         ./src/Backend/Render.cpp                               # Generic window classes
         ./src/Backend/SDLRender/SDLRender.cpp                  # Generic window classes
         ./src/Backend/OpenGLRender/OpenGLRender.cpp            #  Generic window classes
         ./src//Backend/NullRender/NullRender.cpp
         ./src//Backend/OnlyOpenGL/OnlyOpenGL.cpp
         ./src//Backend/SDLBlackPixel/SDLBlackPixel.cpp
         ./src//Backend/SDLColorPixel/SDLColorPixel.cpp
         ./src//Backend/SpriteOpenGL/SpriteOpenGL.cpp
         ./src//Backend/BackendSprite/tools.cpp
         ./src//Backend/BackendSprite/BackendSprite.cpp
         ./src//Backend/BackEndColoredSprite/BackEndColoredSprite.cpp
         ./src/others/text.cpp
         ./src/main.cpp )


add_executable(galax_eirb ${SOURCE_FILES})

target_link_libraries(galax_eirb SDL2)
target_link_libraries(galax_eirb GLEW)
target_link_libraries(galax_eirb GLU)
target_link_libraries(galax_eirb GL)
