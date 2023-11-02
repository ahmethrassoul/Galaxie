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
 #include <metal_stdlib>
using namespace metal;

constant int THREADGROUP_SIZE = 1024; // seems to be a constant !
/*
 *
 *
 *##############################################################################
 *
 *
 */
#define BSIZE 256 			   // Block size in # of threads
#define softeningSquared 0.01f // original plumer softener is 0.025. here the value is square of it.
#define damping          1.00f // 0.999f
#define ep               0.67f // 0.5f
/*
 *
 *
 *##############################################################################
 *
 *
 */

