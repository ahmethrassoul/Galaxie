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
#include "SDLColorPixel.hpp"
#include "../others/text.h"
#include <chrono>
#include <iostream>

SDLColorPixel::SDLColorPixel(const int _width, const int _height, const std::string colorfile) : SDLRender(_width, _height, colorfile)
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
SDLColorPixel::~SDLColorPixel()
{
    SDL_GL_DeleteContext( glWindow );
    DestroyTextRes();
    SDL_DestroyWindow( window );
    SDL_Quit();
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLColorPixel::render(Galaxy* galaxie)
{
//#if 1
//    const float red  [] = {79.f/255.f,  67.f/255.f, 209.f/255.f, 106.f/255.f, 238.f/255.f, 131.f/255.f, 1.f};
//    const float green[] = {53.f/255.f, 105.f/255.f,  47.f/255.f, 142.f/255.f, 125.f/255.f, 168.f/255.f, 1.f};
//    const float blue [] = {95.f/255.f, 167.f/255.f,  94.f/255.f, 193.f/255.f, 138.f/255.f, 202.f/255.f, 1.f};
//#else
//    const float red  [] = {0.53f, 0.70f, 0.43f, 0.60f, 0.33f, 0.50f, 1.0};
//    const float green[] = {0.93f, 0.30f, 0.83f, 0.20f, 0.73f, 0.10f, 1.0};  // colors associated to galaxy parts
//    const float blue [] = {1.00f, 0.20f, 0.90f, 0.10f, 0.80f, 0.00f, 1.0};
//#endif
    int lastColor = 0;
    glColor3f(0.53f,0.93f,1.0f);
    glPointSize( pointSize );
    glBegin(GL_POINTS);

    for(int kl = 0; kl < galaxie->size; kl+= 1)
    {
        if( galaxie->color[kl] != lastColor )
        {
            lastColor = galaxie->color[kl];
            //glEnd();
            glColor3f(colors.red[lastColor], colors.green[lastColor], colors.blue[lastColor]);
            //glPointSize( pointSize );
            //glBegin(GL_POINTS);
        }
        glVertex3f( galaxie->pos_x[kl], galaxie->pos_y[kl], galaxie->pos_z[kl]);
    }
    glEnd();
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
