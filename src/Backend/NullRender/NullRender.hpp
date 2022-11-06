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
#if !defined(_NullRender_)
#define _NullRender_

#include "../Render.hpp"
#include <chrono>

class NullRender : public Render
{
public:
     NullRender(const int _width, const int _height, const std::string colorfile);
    ~NullRender();

    void render(Galaxy* galaxie);
    void refresh_fps(const float fps);
    void refresh_interface();

private:
    int    n_exec;
    float sum_fps;
    float min_fps;
    float max_fps;

    std::chrono::time_point<std::chrono::steady_clock> start;

};

#endif //MAIN_SDL2OPENGL_H
