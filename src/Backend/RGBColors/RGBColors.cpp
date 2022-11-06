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
#include "RGBColors.hpp"
#include <sstream>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RGBColors::RGBColors(const std::string filename)
{
    //
    // On cree la structure pour la galaxie
    //
    std::ifstream ifile( filename );
    if( ifile.is_open() == false )
    {
        std::cout << "(EE) Error opening file (" << filename << ")" << std::endl;
        std::cout << "(EE) FILE (" << __FILE__ << ")" << std::endl;
        std::cout << "(EE) LINE (" << __LINE__ << ")" << std::endl;
        exit( EXIT_FAILURE );
    }

    std::vector<std::string> liste;
    std::string line;
    while( std::getline(ifile, line) )
    {
        if( line.size() != 0 )
        {
            if(line[0] == '#') continue;

            std::string::iterator new_end =
                    std::unique(line.begin(), line.end(),
                                [=](char lhs, char rhs){ return (lhs == rhs) && (lhs == ' '); }
                    );
            line.erase(new_end, line.end());

            liste.push_back( line );
        }
    }
    ifile.close();

    if( liste.size() != 3 )
    {
        std::cout << "(EE) Error the COLOR file does not contain 3 valid lines" << std::endl;
        std::cout << "(EE) FILE (" << __FILE__ << ")" << std::endl;
        std::cout << "(EE) LINE (" << __LINE__ << ")" << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    // On parse les lignes apres les autres et on charge la structure
    //
    std::string buffer;

    std::stringstream ssr( liste[0] );
    while( std::getline(ssr, buffer, ' ') )
    {
        if( buffer.size() == 0 ) continue;
        red.push_back( std::stof( buffer ) / 255.f );
    }

    std::stringstream ssg( liste[1] );
    while( std::getline(ssg, buffer, ' ') )
    {
        if( buffer.size() == 0 ) continue;
        green.push_back( std::stof( buffer ) / 255.f );
    }

    std::stringstream ssb( liste[2] );
    while( std::getline(ssb, buffer, ' ') )
    {
        if( buffer.size() == 0 ) continue;
        blue.push_back( std::stof( buffer ) / 255.f );
    }

    //
    // ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
    //
    if( (red.size() != green.size()) || (green.size() != blue.size()) )
    {
        std::cout << "(EE) The number of values is different !" << std::endl;
        std::cout << "(EE) FILE (" << __FILE__ << ")" << std::endl;
        std::cout << "(EE) LINE (" << __LINE__ << ")" << std::endl;
        std::cout << "(EE) red.size()   = " << red.size()   << std::endl;
        std::cout << "(EE) green.size() = " << green.size() << std::endl;
        std::cout << "(EE) blue.size()  = " << blue.size()  << std::endl;
        exit( EXIT_FAILURE );
    }
}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//
RGBColors::~RGBColors()
{

}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//
//
//