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
 */#include "SDLBlackPixel.hpp"
#include "../others/text.h"
#include <chrono>
#include <iostream>
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
SDLBlackPixel::SDLBlackPixel(const int _width, const int _height, const std::string colorfile) : SDLRender(_width, _height, colorfile)
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
SDLBlackPixel::~SDLBlackPixel()
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLBlackPixel::render(Galaxy* galaxie)
{
    glColor3f(1.0f,1.0f,1.0f);
    glPointSize( pointSize );
    glBegin(GL_POINTS);
    for(int kl = 0; kl < galaxie->size; kl+= 1)
    {
        glVertex3f( galaxie->pos_x[kl], galaxie->pos_y[kl], galaxie->pos_z[kl]);
    }
    glEnd();
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
