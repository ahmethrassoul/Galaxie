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
#include "Galaxy.hpp"
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy::Galaxy(const int _size)
{
	size      = _size;
	pos_x     = (float *)malloc(sizeof(float) * size);
	pos_y     = (float *)malloc(sizeof(float) * size);
	pos_z     = (float *)malloc(sizeof(float) * size);

	pos_x_new = (float *)malloc(sizeof(float) * size);
	pos_y_new = (float *)malloc(sizeof(float) * size);
	pos_z_new = (float *)malloc(sizeof(float) * size);

	mass      = (float *)malloc(sizeof(float) * size);

	vel_x     = (float *)malloc(sizeof(float) * size);
	vel_y     = (float *)malloc(sizeof(float) * size);
	vel_z     = (float *)malloc(sizeof(float) * size);
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy::Galaxy(struct galaxy g)
{
    size = g.size;

    pos_x = (float *)malloc(sizeof(float) * size);
    pos_y = (float *)malloc(sizeof(float) * size);
    pos_z = (float *)malloc(sizeof(float) * size);

    memcpy(pos_x, g.pos_x, sizeof(float) * size);
    memcpy(pos_y, g.pos_y, sizeof(float) * size);
    memcpy(pos_z, g.pos_z, sizeof(float) * size);

    pos_x_new = (float *)malloc(sizeof(float) * size);
    pos_y_new = (float *)malloc(sizeof(float) * size);
    pos_z_new = (float *)malloc(sizeof(float) * size);

    memcpy(pos_x_new, g.pos_x_new, sizeof(float) * size);
    memcpy(pos_y_new, g.pos_y_new, sizeof(float) * size);
    memcpy(pos_z_new, g.pos_z_new, sizeof(float) * size);

    mass = (float *)malloc(sizeof(float) * size);

    memcpy(mass, g.mass, sizeof(float) * size);

    vel_x     = (float *)malloc(sizeof(float) * size);
    vel_y     = (float *)malloc(sizeof(float) * size);
    vel_z     = (float *)malloc(sizeof(float) * size);

    memcpy(vel_x, g.vel_x, sizeof(float) * size);
    memcpy(vel_y, g.vel_y, sizeof(float) * size);
    memcpy(vel_z, g.vel_z, sizeof(float) * size);

    color     = (int8_t*)malloc(sizeof(int8_t) * size);
    memcpy(color, g.color, sizeof(int8_t) * size);

    std::cout << std::setiosflags(std::ios_base::showpos);
    std::cout << "(II) End of a Galaxy object creation" << std::endl;
    std::cout << "(II) -    x range = [" << std::setw(6) << min_x()    << "; " << std::setw(6) << max_x()    << "]" << std::endl;
    std::cout << "(II) -    y range = [" << std::setw(6) << min_y()    << "; " << std::setw(6) << max_y()    << "]" << std::endl;
    std::cout << "(II) -    z range = [" << std::setw(6) << min_z()    << "; " << std::setw(6) << max_z()    << "]" << std::endl;
    std::cout << "(II) -  xyz range = [" << std::setw(6) << min_xyz()  << "; " << std::setw(6) << max_xyz()  << "]" << std::endl;
    std::cout << "(II) - mass range = [" << std::setw(6) << min_mass() << "; " << std::setw(6) << max_mass() << "]" << std::endl;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy::Galaxy(Galaxy& g)
{
    size = g.size;

    pos_x = (float *)malloc(sizeof(float) * size);
    pos_y = (float *)malloc(sizeof(float) * size);
    pos_z = (float *)malloc(sizeof(float) * size);

    memcpy(pos_x, g.pos_x, sizeof(float) * size);
    memcpy(pos_y, g.pos_y, sizeof(float) * size);
    memcpy(pos_z, g.pos_z, sizeof(float) * size);

    pos_x_new = (float *)malloc(sizeof(float) * size);
    pos_y_new = (float *)malloc(sizeof(float) * size);
    pos_z_new = (float *)malloc(sizeof(float) * size);

    memcpy(pos_x_new, g.pos_x_new, sizeof(float) * size);
    memcpy(pos_y_new, g.pos_y_new, sizeof(float) * size);
    memcpy(pos_z_new, g.pos_z_new, sizeof(float) * size);

    mass = (float *)malloc(sizeof(float) * size);

    memcpy(mass, g.mass, sizeof(float) * size);

    vel_x     = (float *)malloc(sizeof(float) * size);
    vel_y     = (float *)malloc(sizeof(float) * size);
    vel_z     = (float *)malloc(sizeof(float) * size);

    memcpy(vel_x, g.vel_x, sizeof(float) * size);
    memcpy(vel_y, g.vel_y, sizeof(float) * size);
    memcpy(vel_z, g.vel_z, sizeof(float) * size);
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Galaxy::~Galaxy()
{
	delete[] pos_x;
	delete[] pos_y;
	delete[] pos_z;

	delete[] pos_x_new;
	delete[] pos_y_new;
	delete[] pos_z_new;

	delete[] mass;

	delete[] vel_x;
	delete[] vel_y;
	delete[] vel_z;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void Galaxy::update()
{
#pragma clang loop vectorize(assume_safety)
	for(int i = 0; i < size; i += 1)
	{
		pos_x[i] = pos_x_new[i];
		pos_y[i] = pos_y_new[i];
		pos_z[i] = pos_z_new[i];
	}
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::min_mass()
{
    if( _min_mass == FLT_MAX )
    {
        for(int i = 0; i < size; i += 1)
        {
            _min_mass = (_min_mass < mass[i]) ? _min_mass : mass[i];
        }
    }
    return _min_mass;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::max_mass()
{
    if( _max_mass == FLT_MIN )
    {
        for(int i = 0; i < size; i += 1)
        {
            _max_mass = (_max_mass > mass[i]) ? _max_mass : mass[i];
        }
    }
    return _max_mass;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::min_x()
{
    float minv = std::abs( pos_x[0] );
    for(int i = 1; i < size; i += 1)
        minv = (minv < pos_x[i]) ? minv : pos_x[i];
    return minv;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::max_x()
{
    float maxv = pos_x[0];
    for(int i = 1; i < size; i += 1)
        maxv = (maxv > pos_x[i]) ? maxv : pos_x[i];
    return maxv;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::min_y()
{
    float minv = pos_y[0];
    for(int i = 1; i < size; i += 1)
        minv = (minv < pos_y[i]) ? minv : pos_y[i];
    return minv;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::max_y()
{
    float maxv = pos_y[0];
    for(int i = 1; i < size; i += 1)
        maxv = (maxv > pos_y[i]) ? maxv : pos_y[i];
    return maxv;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::min_z()
{
    float minv = pos_z[0];
    for(int i = 1; i < size; i += 1)
        minv = (minv < pos_z[i]) ? minv : pos_z[i];
    return minv;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::max_z()
{
    float maxv = pos_z[0];
    for(int i = 1; i < size; i += 1)
        maxv = (maxv > pos_z[i]) ? maxv : pos_z[i];
    return maxv;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::min_xyz()
{
    const float minx = min_x();
    const float miny = min_y();
    const float minz = min_z();
    const float mint = (minx > miny) ? minx : miny;
    return (mint > minz) ? mint : minz;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Galaxy::max_xyz()
{
    const float maxx = max_x();
    const float maxy = max_y();
    const float maxz = max_z();
    const float maxt = (maxx > maxy) ? maxx : maxy;
    return (maxt > maxz) ? maxt : maxz;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
