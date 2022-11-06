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
#include "Kernel.hpp"
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Kernel::Kernel()
{
    execTime  = 0.0;
    nExecTime = 0;

    dt = 0.01f; // valeur par defaut

};
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
Kernel::~Kernel()
{

};
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Kernel::avgExecTime()
{
    return execTime / (double)nExecTime;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void Kernel::startExec()
{
    start = std::chrono::steady_clock::now();
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void Kernel::stopExec()
{
    auto end = std::chrono::steady_clock::now();
    execTime  += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    nExecTime += 1;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
void Kernel::set_dt(const float value)
{
    if( value <= 0.000001f )
    {
        std::cout << "(EE) The Delta-Time parameter seems incorrect (dt = " << value << " <= 0.000001)" << std::endl;
        exit( EXIT_FAILURE );
    }
    if( value >= 10.0f )
    {
        std::cout << "(WW) The Delta-Time parameter seems incorrect (dt = " << value << " >= 10.0f)" << std::endl;
    }
    dt = value;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
float Kernel::get_dt()
{
    return dt;
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
