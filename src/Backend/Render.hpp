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
#ifndef _Render_
#define _Render_
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
#include "../datatype/datatype.h"
#include "../Galaxy/Galaxy.hpp"
#include "./RGBColors/RGBColors.hpp"
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
class Render
{
protected:
    int width;
    int height;

protected:
    bool finished = false;
    RGBColors colors;

public:
             Render(const int _width, const int _height, const std::string colorfile);
    virtual ~Render();

    virtual bool isFinished() final;

    virtual void render       (Galaxy* galaxie  ) = 0;
    virtual void refresh_fps  (const float fps  ) = 0;
    virtual void refresh_interface()              = 0;
};
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
#endif