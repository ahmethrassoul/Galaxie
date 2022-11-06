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
#if !defined(_OpenGLRender_) && defined(_ENABLE_SDL_)
#define _OpenGLRender_
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
#include "../Render.hpp"
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
class OpenGLRender : public Render
{
private:
    bool mode_sdl;

protected:
    SDL_Event       event;
    SDL_Window*     window;
    SDL_DisplayMode current;
    SDL_GLContext   glWindow;

protected:
    float mouseOriginX = 0.0f;
    float mouseOriginY = 0.0f;
    float mouseMoveX   = 0.0f;
    float mouseMoveY   = 0.0f;
    float mouseDeltaX  = 0.0f;
    float mouseDeltaY  = 0.0f;

protected:
    float oldCamPos[3] = { 0.0f, 0.0f, -45.0f };
    float oldCamRot[3] = { 0.0f, 0.0f,   0.0f };
    float newCamPos[3] = { 0.0f, 0.0f, -45.0f };
    float newCamRot[3] = { 0.0f, 0.0f,   0.0f };

protected:
    float  pointSize = 1.0f;
    float spriteSize = 1.0f;
    const float scaleFactor = 1.5f;	// constant from J's code

    bool enableGrid = true;
    bool enableAxes = true;

public:
             OpenGLRender(const int _width, const int _height, const std::string colorfile, const bool _mode_sdl = true);
    virtual ~OpenGLRender();

    virtual void refresh_interface() final;
    virtual void refresh_fps  (const float fps  );

    virtual void setGridEnable(const bool enable) final;
    virtual void setAxesEnable(const bool enable) final;

    virtual void DrawGridXZ() final;
    virtual void ShowAxes  () final;

    virtual void setPointSize (const float value) final;
    virtual void setSpriteSize(const float value) final;
};
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
#endif