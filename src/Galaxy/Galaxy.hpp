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
#ifndef _Galaxie_
#define _Galaxie_

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <cfloat>
#include <chrono>
#include <vector>
#include <cmath>
#include <iostream>

#include "../datatype/datatype.h"
#include "GL/glew.h"

class Galaxy
{
public:	
	Galaxy(int particules);
	Galaxy(struct galaxy g);
    Galaxy(Galaxy& g);

    ~Galaxy();

	void update();

	float* pos_x;
	float* pos_y;
	float* pos_z;
	float* mass;
	
	float* vel_x;
	float* vel_y;
	float* vel_z;

	float* pos_x_new;
	float* pos_y_new;
	float* pos_z_new;


    int8_t* color;

    int size;

    float min_mass();
    float max_mass();

    float min_x  ();
    float min_y  ();
    float min_z  ();
    float min_xyz();

    float max_x  ();
    float max_y  ();
    float max_z  ();
    float max_xyz();

private:
    float _min_mass = FLT_MAX;
    float _max_mass = FLT_MIN;

};

#endif

