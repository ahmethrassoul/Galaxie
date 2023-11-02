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

#ifdef __SSE4_2__
TEST_CASE( "NAIVE vs INTEL SSE4", "[sse4]" )
{
    std::string filename = "../galaxies/dubinski_colored.tab";

    galaxy galaxie = LoadFromFile( filename, step );
    if( galaxie.size == 0 )
    {
        std::cout << "(EE) Error the galaxy has no paricule (g.size == " << galaxie.size << ")" << std::endl;
        exit( EXIT_FAILURE );
    }

    Kernel* k_ref = new RenderNaive( galaxie );
    Kernel* k_mod = new RenderSSE4 ( galaxie );

    delete_galaxy( galaxie  );

    auto start_ref = std::chrono::steady_clock::now();
    for(int i = 0; i < iterations; i += 1)
    {
        k_ref->execute();
    }
    auto end_ref = std::chrono::steady_clock::now();

    auto start_mod = std::chrono::steady_clock::now();
    for(int i = 0; i < iterations; i += 1)
    {
        k_mod->execute();
    }
    auto end_mod = std::chrono::steady_clock::now();

    auto execTime_ref   = std::chrono::duration_cast<std::chrono::milliseconds>(end_ref - start_ref).count();
    auto execTime_mod   = std::chrono::duration_cast<std::chrono::milliseconds>(end_mod - start_mod).count();

    const float fps_ref =    (float)step * (1000.0f / execTime_ref);
    const float fps_mod =    (float)step * (1000.0f / execTime_mod);
    const float speedup = fps_mod / fps_ref;

    Galaxy* gr = k_ref->particules();
    Galaxy* gn = k_mod->particules();

    float maxErr_x = 0.f;
    float maxErr_y = 0.f;
    float maxErr_z = 0.f;

    float sumErr_x = 0.f;
    float sumErr_y = 0.f;
    float sumErr_z = 0.f;

    const int size = gr->size;
    for(int i = 0; i < size; i += 1)
    {
        const float diff_x = std::abs( gr->pos_x[i] - gn->pos_x[i] );
        const float diff_y = std::abs( gr->pos_y[i] - gn->pos_y[i] );
        const float diff_z = std::abs( gr->pos_z[i] - gn->pos_z[i] );

        sumErr_x += diff_x;
        sumErr_y += diff_y;
        sumErr_z += diff_z;

        maxErr_x = std::max(maxErr_x, diff_x);
        maxErr_y = std::max(maxErr_y, diff_y);
        maxErr_z = std::max(maxErr_z, diff_z);
    }

    float avgErr_x = sumErr_x / (float)(size);
    float avgErr_y = sumErr_y / (float)(size);
    float avgErr_z = sumErr_z / (float)(size);

    printf("[sse4     ] Max. errors (%1.5f, %1.5f, %1.5f) | Avg. errors (%1.5f, %1.5f, %1.5f) | FPS %7.3f | Speed-up %7.3f |\n", maxErr_x, maxErr_y, maxErr_z, avgErr_x, avgErr_y, avgErr_z, fps_mod, speedup);

    delete k_ref;
    delete k_mod;

    const bool max_error = (maxErr_x < epsilon) && (maxErr_y < epsilon) && (maxErr_z < epsilon);
    REQUIRE( max_error == true );
}
#endif
