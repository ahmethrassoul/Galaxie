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
#include "NullRender.hpp"
#include "../others/text.h"
#include <chrono>
#include <iostream>
#include <cstdint>
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
NullRender::NullRender(const int _width, const int _height, const std::string colorfile) : Render(_width, _height, colorfile)
{
     n_exec =    0;
    sum_fps = 0.0f;
    min_fps = std::numeric_limits<float>::max();
    max_fps = std::numeric_limits<float>::min();
    start   = std::chrono::steady_clock::now();
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
NullRender::~NullRender()
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void NullRender::render(Galaxy* galaxie)
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void NullRender::refresh_fps(const float fps)
{

     n_exec        +=    1;
    sum_fps        += fps;
    min_fps         = std::min( min_fps, fps );
    max_fps         = std::max( max_fps, fps );
    const float avg = sum_fps /(float)n_exec;

    auto end = std::chrono::steady_clock::now();
    auto exe = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    if( exe > 5 ) // every 5 seconds
    {
        printf("Performance | min = %7.3f fps | max = %7.3f fps | avg = %7.3f fps | curr = %7.3f fps |\n", min_fps, max_fps, avg, fps);
        start = std::chrono::steady_clock::now();
    }
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void NullRender::refresh_interface( )
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//