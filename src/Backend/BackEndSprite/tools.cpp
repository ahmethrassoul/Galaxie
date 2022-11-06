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
#include "tools.hpp"
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
char* __attribute__((noinline)) textFileRead(const char *fn)
{
    char* content = nullptr;

    int count = 0;

    FILE* fp = fopen(fn,"rt");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        count = ftell(fp);
        rewind(fp);

        if (count > 0)
        {
            content = (char *)malloc(sizeof(char) * (count+1));
            count   = fread(content,sizeof(char),count,fp);
            content[count] = '\0';
        }
        fclose(fp);
    }

    if (content == NULL)
    {
        fprintf(stderr, "ERROR: could not load in file %s\n", fn);
    }else{
        fprintf(stderr, "The sprite was loaded %d bytes\n", count);
    }

    return content;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
inline float evalHermite(float pA, float pB, float vA, float vB, float u)
{
    float u2=(u*u), u3=u2*u;
    float B0 =  2 * u3 - 3 * u2 + 1;
    float B1 = -2 * u3 + 3 * u2;
    float B2 =      u3 - 2 * u2 + u;
    float B3 =      u3 -          u;
    return( B0*pA + B1*pB + B2*vA + B3*vB );
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
unsigned char* createGaussianMap(const int N)
{
    float *M = new float[2*N*N];
    unsigned char *B = new unsigned char[4*N*N]; 
    float X,Y,Y2,Dist;
    float Incr = 2.0f/N;
    int i=0;
    int j = 0;
    Y = -1.0f;
    //float mmax = 0;
    for (int y=0; y<N; y++, Y+=Incr)
    {
        Y2=Y*Y;
        X = -1.0f;
        for (int x=0; x<N; x++, X+=Incr, i+=2, j+=4)
        {
            Dist = (float)sqrtf(X*X+Y2);
            if (Dist>1) Dist=1;
            M[i+1] = M[i] = evalHermite(1.0f,0,0,0,Dist);
            B[j+3] = B[j+2] = B[j+1] = B[j] = (unsigned char)(M[i] * 255);
        }
    }
    delete [] M;
    return(B);
}