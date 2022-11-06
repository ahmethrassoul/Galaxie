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
#include "BackEndSprite.hpp"
#include "../others/text.h"
#include "tools.hpp"
#include <chrono>
#include <iostream>
#if defined(__APPLE__) || defined(MACOSX)
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#define PARTICLE_POINTS 0
#define PARTICLE_SPRITES 1
#define PARTICLE_SPRITES_COLOR 2
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BackEndSprite::BackEndSprite(const int _width, const int _height, const std::string colorfile) : OpenGLRender(_width, _height, colorfile)
{
    //
    // On cree les textures utilisées par la suite pour les particules
    //

    const int resolution = 32;
    unsigned char* data = createGaussianMap(resolution);
    glGenTextures(1, (GLuint*)&m_texture);
    glBindTexture  (GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA8, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete [] data;

    glewInit();

    if (!glewIsSupported("GL_VERSION_2_0 "
                         "GL_VERSION_1_5 "
                         "GL_ARB_multitexture "
                         "GL_ARB_vertex_buffer_object"))
    {
        fprintf(stderr, "Required OpenGL extensions missing.");
        exit( EXIT_FAILURE );
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

#if 0
    const char* vert = "../data/sprite.vert";
    const char* frag = "../data/sprite.frag";
          char* vs = textFileRead(vert);
          char* fs = textFileRead(frag);
#else
    char* vs = textFileRead("../data/sprite.vert");
    if( vs == nullptr )
    {
        vs = textFileRead("./data/sprite.vert");
        if( vs == nullptr )
        {
            printf( "(EE)\n" );
            printf( "(EE) Unable to init SPRITE resources required for GUI.\n" );
            printf( "(EE) -> file (../data/sprite.vert) is missing !\n");
            printf( "(EE)    and\n" );
            printf( "(EE) -> file (./data/sprite.vert) is missing too...\n");
            printf( "(EE)\n" );
            exit( EXIT_FAILURE );
        }
    }

    char* fs = textFileRead("../data/sprite.frag");
    if( fs == nullptr )
    {
        fs = textFileRead("./data/sprite.frag");
        if( fs == nullptr )
        {
            printf( "(EE)\n" );
            printf( "(EE) Unable to init SPRITE resources required for GUI.\n" );
            printf( "(EE) -> file (../data/sprite.frag) is missing !\n");
            printf( "(EE)    and\n" );
            printf( "(EE) -> file (./data/sprite.frag) is missing too...\n");
            printf( "(EE)\n" );
            exit( EXIT_FAILURE );
        }
    }
#endif

    const char* vv = vs;
    const char* ff = fs;

    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);

    free(vs);
    free(fs);

    glCompileShader(v);
    glCompileShader(f);

    GLuint pro = glCreateProgram();
    glAttachShader(pro,v);
    glAttachShader(pro,f);

    glLinkProgram(pro);

    m_program = pro;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BackEndSprite::~BackEndSprite()
{

}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void BackEndSprite::render(Galaxy* galaxie)
{
    glEnable   (GL_POINT_SPRITE_ARB);
    glTexEnvi  (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable   (GL_VERTEX_PROGRAM_POINT_SIZE_NV);
    glPointSize(spriteSize);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable   (GL_BLEND);
    glDepthMask(GL_FALSE);

    glUseProgram(m_program);
    glUniform1i(glGetUniformLocation(m_program,"mode"), PARTICLE_SPRITES);
    GLuint texLoc = glGetUniformLocation(m_program, "splatTexture");
    glUniform1i(texLoc, 0);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, m_texture);


    const float red  [] = {0.53f, 0.70f, 0.43f, 0.60f, 0.33f, 0.50f, 1.0};
    const float green[] = {0.93f, 0.30f, 0.83f, 0.20f, 0.73f, 0.10f, 1.0};
    const float blue [] = {1.00f, 0.20f, 0.90f, 0.10f, 0.80f, 0.00f, 1.0};

    int lastColor = -1;

    glBegin(GL_POINTS);
    for(int kl = 0; kl < galaxie->size; kl+= 1)
    {
        const int color = galaxie->color[kl];
        if( color != lastColor )
        {
            glColor3f(red[color], green[color], blue[color]);
            lastColor = color;
        }
        glVertex3f( galaxie->pos_x[kl], galaxie->pos_y[kl], galaxie->pos_z[kl]);
    }
    glEnd();

    glUseProgram(0);

    glDisable(GL_POINT_SPRITE_ARB);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
