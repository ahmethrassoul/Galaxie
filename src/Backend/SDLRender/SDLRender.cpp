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
#include "SDLRender.hpp"
#include "../others/text.h"

#define LIMIT(x,min,max) { if ((x)>(max)) (x)=(max); if ((x)<(min)) (x)=(min);}

//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
SDLRender::SDLRender(const int _width, const int _height, const std::string colorfile) : Render(_width, _height, colorfile)
{
    if ( SDL_Init ( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "Error: unable to execute SDL_INIT_EVERYTHING\n" );
        exit( EXIT_FAILURE );
    }

    if ( SDL_GetDesktopDisplayMode( 0, &current ) )
    {
        printf( "Error: unable to execute SDL_GetDesktopDisplayMode\n" );
        exit( EXIT_FAILURE );
    }

    window = SDL_CreateWindow( "GalaxEirb window",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width, height,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

    glWindow = SDL_GL_CreateContext( window );
    SDL_GL_MakeCurrent( window, glWindow );

    GLenum status = glewInit();

    if ( status != GLEW_OK ) {
        printf( "error: unable to execute glewInit\n" );
        exit( EXIT_FAILURE );
    }

    const char* police = "../data/DroidSans.ttf";
    if ( ! InitTextRes( police ) )
    {
        const char* polic2 = "../data/DroidSans.ttf";
        if ( ! InitTextRes( polic2 ) )
        {
            printf( "(EE)\n" );
            printf( "(EE) Unable to init text resources required for GUI.\n" );
            printf( "(EE) -> file (%s) is missing !\n", police);
            printf( "(EE)    and\n" );
            printf( "(EE) -> file (%s) is missing too...\n", polic2);
            printf( "(EE)\n" );
            exit( EXIT_FAILURE );
        }
    }

    SDL_GL_SetSwapInterval( 1 );}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
SDLRender::~SDLRender()
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
void SDLRender::setPointSize (const float value)
{
    pointSize = value;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::setSpriteSize(const float value)
{
    spriteSize = value;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::setGridEnable(const bool enable)
{
    enableGrid = enable;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::setAxesEnable(const bool enable)
{
    enableAxes = enable;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::DrawGridXZ ()
{
    const float ox = -100.0f;
    const float oy =    0.0f;
    const float oz = -100.0f;
    const int   w  =   20;
    const int   h  =   20;
    const float sz =   10.0f;

    glLineWidth( 1.0f );

    glBegin( GL_LINES );

    glColor3f( 0.48f, 0.48f, 0.48f );

    for (int i = 0; i <= h; i += 1 )
    {
        glVertex3f( ox, oy, oz + i * sz );
        glVertex3f( ox + w * sz, oy, oz + i * sz );
    }

    for (int i = 0; i <= h; i += 1 )
    {
        glVertex3f( ox + i * sz, oy, oz );
        glVertex3f( ox + i * sz, oy, oz + h * sz );
    }

    glEnd();
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::ShowAxes()
{
    glLineWidth( 2.0f );
    glBegin( GL_LINES );

    glColor3f ( 1.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 2.0f, 0.0f, 0.0f );

    glColor3f ( 0.0f, 1.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 2.0f, 0.0f );

    glColor3f ( 0.0f, 0.0f, 1.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 2.0f );

    glEnd();
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::refresh_interface()
{
    const float g_inertia = 0.5f;

    while ( SDL_PollEvent( &event ) )
    {
        unsigned int e = event.type;

        if ( e == SDL_MOUSEMOTION ) {

            mouseMoveX = event.motion.x;
            mouseMoveY = height - event.motion.y - 1;

        } else if ( e == SDL_KEYDOWN ) {

            if ( event.key.keysym.sym == SDLK_F1 ) {
                setGridEnable( !enableGrid );

            } else if ( event.key.keysym.sym == SDLK_F2 ) {
                setAxesEnable( !enableAxes );

            } else if ( (event.key.keysym.sym == SDLK_PLUS) || (event.key.keysym.sym == 'p') ) {
                pointSize += scaleFactor * 0.02f;
                LIMIT(pointSize, 1.0f, scaleFactor * 2.0f);

                spriteSize += scaleFactor * 0.02f;
                LIMIT(spriteSize, 0.1f, scaleFactor * 2.0f);
                printf("(+) pointSize = %1.5f | spriteSize = %1.5f\n", pointSize, spriteSize);

            } else if ( (event.key.keysym.sym == SDLK_MINUS) || (event.key.keysym.sym == 'm') ) {
                pointSize -= scaleFactor * 0.02f;
                LIMIT(pointSize, 1.0f, scaleFactor * 2.0f);

                spriteSize -= scaleFactor * 0.02f;
                LIMIT(spriteSize, 0.1f, scaleFactor * 2.0f);
                printf("(-) pointSize = %1.5f | spriteSize = %1.5f\n", pointSize, spriteSize);

            } else if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                finished = true;
            }
        }

        if ( e == SDL_QUIT ) {
            printf( "quit\n" );
            finished = true;
        }
    }

    //
    ////////////////////////////////////////////////////////////////////////////////
    //

    mouseDeltaX = mouseMoveX - mouseOriginX;
    mouseDeltaY = mouseMoveY - mouseOriginY;

    if ( SDL_GetMouseState( 0, 0 ) & SDL_BUTTON_LMASK ) {
        oldCamRot[ 0 ] += -mouseDeltaY / 5.0f;
        oldCamRot[ 1 ] +=  mouseDeltaX / 5.0f;
    }else if ( SDL_GetMouseState( 0, 0 ) & SDL_BUTTON_RMASK ) {
        oldCamPos[ 2 ] += ( mouseDeltaY / 100.0f ) * 0.5 * fabs( oldCamPos[ 2 ] );
        oldCamPos[ 2 ]  = oldCamPos[ 2 ] > -5.0f ? -5.0f : oldCamPos[ 2 ];
    }

    mouseOriginX = mouseMoveX;
    mouseOriginY = mouseMoveY;

    //
    ////////////////////////////////////////////////////////////////////////////////
    //

    glViewport( 0, 0, width, height );
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_BLEND );
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

#if 0 //__APPLE__
    glMultMatrix( GLKMatrix4MakePerspective( 50.0f, (float)width / (float)height, 0.1f, 100000.0f ).m ); // << .m is the GLfloat* you are accessing
#else
    gluPerspective( 50.0f, (float)width / (float)height, 0.1f, 100000.0f );
#endif
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //
    ////////////////////////////////////////////////////////////////////////////////
    //

    for ( int i = 0; i < 3; ++i )
    {
        newCamPos[ i ] += ( oldCamPos[ i ] - newCamPos[ i ] ) * g_inertia;
        newCamRot[ i ] += ( oldCamRot[ i ] - newCamRot[ i ] ) * g_inertia;
    }

    glTranslatef( newCamPos[0], newCamPos[1], newCamPos[2] );
    glRotatef( newCamRot[0], 1.0f, 0.0f, 0.0f );
    glRotatef( newCamRot[1], 0.0f, 1.0f, 0.0f );

    if ( enableGrid ) DrawGridXZ();
    if ( enableAxes ) ShowAxes();

    //
    ////////////////////////////////////////////////////////////////////////////////
    //
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void SDLRender::refresh_fps(const float avgFPS)
{
    char sfps[40] = "FPS: ";
    sprintf( sfps, "FPS : %.4f", avgFPS );

    //
    ////////////////////////////////////////////////////////////////////////////////
    //

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#if __APPLE__
    gluOrtho2D(0, width, 0, height);
#else
    gluOrtho2D(0, width, 0, height);
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    DrawText( 10, height - 20, sfps,           TEXT_ALIGN_LEFT, RGBA(255, 255, 255, 255) );
    DrawText( 10, 30, "'F1' : show/hide grid", TEXT_ALIGN_LEFT, RGBA(255, 255, 255, 255) );
    DrawText( 10, 10, "'F2' : show/hide axes", TEXT_ALIGN_LEFT, RGBA(255, 255, 255, 255) );
    SDL_UpdateWindowSurface( window );

    //
    ////////////////////////////////////////////////////////////////////////////////
    //

#if __APPLE__
    const auto beforeSwap = std::chrono::system_clock::now();
#endif
    SDL_GL_SwapWindow(window);
#if __APPLE__
    const auto afterSwap = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(afterSwap - beforeSwap).count() < 3.0)
        SDL_Delay(10);
#endif
}
