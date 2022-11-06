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
#include "OnlyOpenGL.hpp"
#include "../others/text.h"
#include <chrono>
#include <iostream>
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
OnlyOpenGL::OnlyOpenGL(const int _width, const int _height, const std::string colorfile) : Render(_width, _height, colorfile)
{
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Chargement du sprite correspondant aux étoiles
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////

    SDL_Surface* tex = SDL_LoadBMP("../data/sprite/particle.bmp");
    if( tex == nullptr )
    {
        SDL_Surface* tex = SDL_LoadBMP("./data/sprite/particle.bmp");
        if( tex == nullptr )
        {
            printf( "(EE)\n" );
            printf( "(EE) Unable to init text resources required for GUI.\n" );
            printf( "(EE) -> file (../data/sprite/particle.bmp) is missing !\n");
            printf( "(EE)    and\n" );
            printf( "(EE) -> file (./data/sprite/particle.bmp) is missing too...\n");
            printf( "(EE)\n" );
            exit( EXIT_FAILURE );
        }
    }

    // texture loading taken from http://gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL

    // get the number of channels in the SDL surface
    GLint  nOfColors = tex->format->BytesPerPixel;
    GLenum texture_format = GL_RGBA;
    if (nOfColors == 4)     // contains an alpha channel
    {
        if ( tex->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }
    else if (nOfColors == 3)     // no alpha channel
    {
        if ( tex->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &m_texStar);

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, m_texStar );

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 nOfColors,
                 tex->w,
                 tex->h,
                 0,
                 texture_format,
                 GL_UNSIGNED_BYTE,
                 tex->pixels );
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
OnlyOpenGL::~OnlyOpenGL()
{
    glDeleteTextures(1,&m_texStar);
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void OnlyOpenGL::render(Galaxy* galaxie)
{
    float maxSize = 0.0f;
    glBindTexture(GL_TEXTURE_2D, m_texStar);
    glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxSize);
    glTexEnvf  (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable   (GL_POINT_SPRITE_ARB);
    glEnable   (GL_TEXTURE_2D);       // point sprite texture support
    glEnable   (GL_BLEND);            // soft blending of point sprites
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//    glPointSize(5); //pStars[i].m_mag*10);
//    glBegin(GL_POINTS);
    float offset = galaxie->min_mass();
    float scale  = std::round(10.0f / (galaxie->max_mass() - offset));

    const float red  [] = {0.8, 0.2, 1.0};
    const float green[] = {0.5, 0.5, 1.0};
    const float blue [] = {0.2, 0.8, 1.0};

    for(int i = 0; i < galaxie->size; i += 1)
    {
        glPointSize(scale * (galaxie->mass[i] - offset) + 1.0f); //pStars[i].m_mag*10);
        glBegin(GL_POINTS);
        glPushMatrix();
        glVertex3f( galaxie->pos_x[i], galaxie->pos_y[i], galaxie->pos_z[i]);
        int color = galaxie->color[i];
        glColor3f(red[color], green[color], blue[color]);
        glVertex3f( galaxie->pos_x[i], galaxie->pos_y[i], galaxie->pos_z[i]);
        glPopMatrix();
        glEnd();
    }
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
