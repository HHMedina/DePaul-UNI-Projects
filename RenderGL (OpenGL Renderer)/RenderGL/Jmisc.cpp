// jmisc.cpp

#define NOMINMAX

#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "jmisc.h"
#include <windows.h>

#ifndef VectorLength
#define VectorLength(n) (sqrt(sqr((n)[0]) + sqr((n)[1]) + sqr((n)[2])))
#endif

bool IsWhitespace(char k)
{
    return ((k == ' ') || (k == '\t') || (k == '\n') || (k == '\r') || (k == '\f') ||
            (k == '\v'));
}

void jMessageBox(char *message, ...)
{
     char s[256];

     va_list list;
     va_start(list, message);

     vsprintf(s, message, list);
     printf(s);
     printf("\n");

     MessageBeep(MB_ICONEXCLAMATION);
     MessageBox(GetActiveWindow(), s, "GraphicsWin", MB_OK | MB_ICONEXCLAMATION);
	
     va_end(list);
}

void complain(char *message, ...)
{
	char s[256];

	va_list list;
	va_start(list, message);

	vsprintf(s, message, list);

#ifndef ADVANCED_ERRORS
	printf("%s\n", s);
	MessageBox(NULL, s, "Error", MB_OK);
	exit(1);
#else
	throw s;
#endif
}

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-15
#define RNMX (1.0 - EPS)

static long seed;

void sran1(long Seed)
{
    seed = -Seed;
}

double ran1(void)
{
    int j; 
    long k;
    static long iy = 0;
    static long iv[NTAB];
    double temp;

    if (seed <= 0 || !iy)
    {
        if (-seed < 1)
            seed = 1;
        else
            seed = -seed;
        for (j = NTAB+7; j >= 0; j--)
        {
            k = seed / IQ;
            seed = IA * (seed - k * IQ) - IR * k;
            if (seed < 0)
                seed += IM;
            if (j < NTAB) 
                iv[j] = seed;
        }
        iy = iv[0];
    }
    k = seed / IQ;
    seed = IA * (seed - k * IQ) - IR * k;
    if (seed < 0)
        seed  += IM;
    j = iy / NDIV;
    iy = iv[j];
    iv[j] = seed;
    if ((temp = AM * iy) > RNMX)
        return RNMX;
    else
        return temp;
}

double normalRan(void)
{
    static int iset = 0;
    static double gset;
    double fac, rsq, v1, v2;

    if (seed < 0)
        iset = 0;
    if (iset == 0)
    {
        do
        {
            v1 = 2.0 * ran1() - 1.0;
            v2 = 2.0 * ran1() - 1.0;
            rsq = v1*v1 + v2*v2;
        } while (rsq >= 1.0 || rsq == 0);
        fac = sqrt(-2.0 * log(rsq) / rsq);
        gset = v1 * fac;
        iset = 1;
        return v2 * fac;
    }
    else
    {
        iset = 0;
        return gset;
    }
}














