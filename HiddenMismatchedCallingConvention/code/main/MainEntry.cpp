#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include <cstdio>

using namespace std;

#pragma optimize( "", off ) 
void callModuleFunc( int v )
{
    HMODULE hModule{ ::LoadLibraryW( L"module.dll" ) };
    if ( nullptr == hModule ) {
        printf( "LoadLibrary() failed: 0x%08x\n", ::GetLastError() );
        return;
    }

    using ModuleFunc = int (*)( int );
    auto fpModuleFunc = reinterpret_cast<ModuleFunc>(
        ::GetProcAddress( hModule, "moduleFunc" ) );
    if ( nullptr == fpModuleFunc ) {
        printf( "GetProcAddress() failed: 0x%08x\n", ::GetLastError() );
    }
    else {
        fpModuleFunc( v );
    }

    ::FreeLibrary( hModule );
}
#pragma optimize( "", on )

int wmain( int argc, wchar_t* argv[] )
{
    callModuleFunc( 10 );

    return 0;
}