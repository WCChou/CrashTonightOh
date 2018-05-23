#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include <cstdio>

extern "C"
int __stdcall moduleFunc( int v )
{
    printf( "moduleFunc got %d\n", v );
    return v + 1;
}