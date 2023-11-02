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
#include "Loader.hpp"


struct galaxy LoadTabFromFile(const std::string filename, const int step)
{
    //
    // On cree la structure pour la galaxie
    //
    std::ifstream ifile( filename );
    if( ifile.is_open() == false )
    {
        std::cout << "(EE) Error opening file (" << filename << ")" << std::endl;
        exit( EXIT_FAILURE );
    }
    //
    // On lit les lignes du fichier une par une mais on filtre step by step
    //
    std::vector<std::string> liste;
    int lineNumber = 0;
    std::string line;
    while( std::getline(ifile, line) )
    {
        if( lineNumber%step == 0 )
        {
            liste.push_back( line );
        }
        lineNumber += 1;
    }
    //
    //
    /////////////////////////////////////////////////////////////////////////////
    //
    //
    if( liste.size() == 0 )
    {
        std::cout << "(EE) Error the file is empty, no particule is loaded" << std::endl;
        exit( EXIT_FAILURE );
    }
    //
    // On parse les lignes apres les autres et on charge la structure
    //
    galaxy g = create_galaxy( liste.size() );
    for(int i = 0; i < (int)liste.size(); i += 1)
    {
        std::string buffer;
        std::stringstream ss( liste[i] );
        std::getline(ss, buffer, ' '); g.mass [i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.pos_x[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.pos_y[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.pos_z[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.vel_x[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.vel_y[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.vel_z[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.color[i] = std::stoi( buffer );
    }
    g.size = liste.size();
    //
    // ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... 
    //
//  std::cout << "(II) The puncturing is set to (" << step << ")"   << std::endl;
//  std::cout << "(II) The file was loaded (" << filename << ")"   << std::endl;
//  std::cout << "(II) # of particules in file = " << lineNumber   << std::endl;
//  std::cout << "(II) # of loaded particules  = " << liste.size() << std::endl;

    return g;
}

struct galaxy LoadGxyFromFile(const std::string filename, const int step)
{
    //
    // On cree la structure pour la galaxie
    //
    std::ifstream ifile( filename );
    if( ifile.is_open() == false )
    {
        std::cout << "(EE) Error opening file (" << filename << ")" << std::endl;
        exit( EXIT_FAILURE );
    }


    //
    // On lit les lignes du fichier une par une mais on filtre step by step
    //


    std::vector<std::string> liste;
    int lineNumber = 0;
    std::string line;
    while( std::getline(ifile, line) )
    {
        if( lineNumber%step == 0 )
        {
            liste.push_back( line );
        }
        lineNumber += 1;
    }

    if( liste.size() == 0 )
    {
        std::cout << "(EE) Error the file is empty, no particule is loaded" << std::endl;
        exit( EXIT_FAILURE );
    }

    //
    // On parse les lignes apres les autres et on charge la structure
    //
    galaxy g = create_galaxy( liste.size() );
    for(int i = 0; i < (int)liste.size(); i += 1)
    {
        std::string buffer;
        std::stringstream ss( liste[i] );

        std::getline(ss, buffer, ' '); g.pos_x[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.pos_y[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.pos_z[i] = std::stof( buffer );

        std::getline(ss, buffer, ' '); g.vel_x[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.vel_y[i] = std::stof( buffer );
        std::getline(ss, buffer, ' '); g.vel_z[i] = std::stof( buffer );

        std::getline(ss, buffer, ' '); g.mass [i] = std::stof( buffer );

        std::getline(ss, buffer, ' '); g.color[i] = std::stoi( buffer );
#if 0
        printf("%1.3f | ", g.mass [i]);
        printf("%1.3f | %1.3f | %1.3f | ",  g.pos_x[i], g.pos_y[i], g.pos_y[i]);
        printf("%1.3f | %1.3f | %1.3f |\n", g.vel_x[i], g.vel_y[i], g.vel_z[i]);
#endif
    }
    g.size = liste.size();


    //
    // ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
    //

    std::cout << "(II) The puncturing is set to (" << step << ")"   << std::endl;
    std::cout << "(II) The file was loaded (" << filename << ")"   << std::endl;
    std::cout << "(II) # of particules in file = " << lineNumber   << std::endl;
    std::cout << "(II) # of loaded particules  = " << liste.size() << std::endl;

    return g;
}

struct galaxy LoadFromFile(const std::string filename, const int step)
{
    if( filename.find(".gxy") != std::string::npos )
    {
        return LoadGxyFromFile(filename, step);
    }
    else if( filename.find(".tab") != std::string::npos )
    {
        return LoadTabFromFile(filename, step);
    }
    else
    {
        std::cout << "(EE) The file format is not yet supported " << filename << std::endl;
        exit( EXIT_FAILURE );
    }
}