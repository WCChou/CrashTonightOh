#include <Common/Config/Config.hpp>

#include <cstdio>
using namespace std;

int wmain( int argc, wchar_t* argv[] )
{
    printf( "            argc: %d\n", argc );

    for ( size_t i = 0; i < argc; ++i ) {
        printf( "       argv[ %d ]: %S\n", i, argv[ i ] );
    }

    printf( "GetCommandLine(): %S\n", ::GetCommandLineW() );
    getchar();

    return 0;
}