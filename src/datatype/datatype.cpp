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
#include "datatype.h"
#include <cstdlib>
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
struct galaxy create_galaxy(const int size)
{
	struct galaxy g;

	g.pos_x     = (float *)malloc(sizeof(float) * size);
	g.pos_y     = (float *)malloc(sizeof(float) * size);
	g.pos_z     = (float *)malloc(sizeof(float) * size);

	g.pos_x_new = (float *)malloc(sizeof(float) * size);
	g.pos_y_new = (float *)malloc(sizeof(float) * size);
	g.pos_z_new = (float *)malloc(sizeof(float) * size);

	g.mass      = (float *)malloc(sizeof(float) * size);

	g.vel_x     = (float *)malloc(sizeof(float) * size);
	g.vel_y     = (float *)malloc(sizeof(float) * size);
	g.vel_z     = (float *)malloc(sizeof(float) * size);

    g.color     = (uint8_t*)malloc(sizeof(uint8_t) * size);

    return g;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void delete_galaxy(struct galaxy g)
{
	free( g.pos_x );
	free( g.pos_y );
	free( g.pos_z );

	free( g.pos_x_new );
	free( g.pos_y_new );
	free( g.pos_z_new );

	free( g.mass );

	free( g.vel_x );
	free( g.vel_y );
	free( g.vel_z );

    free( g.color );
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void update_galaxy(struct galaxy g)
{
	const int size = g.size;
#pragma clang loop vectorize(assume_safety)
	for(int i = 0; i < size; i += 1)
	{
		g.pos_x[i] = g.pos_x_new[i];
		g.pos_y[i] = g.pos_y_new[i];
		g.pos_z[i] = g.pos_z_new[i];
	}
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
