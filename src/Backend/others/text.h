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
#ifndef __TEXT_H__
#define __TEXT_H__

#define RGBA(r,g,b,a) (r) | (g << 8) | (b << 16) | (a << 24)

#define TEXT_ALIGN_LEFT 0
#define TEXT_ALIGN_RIGHT 1
#define TEXT_ALIGN_CENTER 2

bool InitTextRes( const char * font );
void DestroyTextRes();
void DrawText( float x, float y, const char *text, int align, unsigned int col );

#endif

