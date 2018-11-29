#include <Common/Config/Config.hpp>

#include <cstdio>
#include <shellapi.h>

int fakeMain( int argc, wchar_t* argv[] )
{
    printf( "            argc: %d\n", argc );

    for ( size_t i = 0; i < argc; ++i ) {
        printf( "       argv[ %d ]: %S\n", i, argv[ i ] );
    }

    printf( "GetCommandLine(): %S\n", ::GetCommandLineW() );
    getchar();

    return 0;
}

int WINAPI wWinMain( HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t* pCmdLine, int nShow )
{
    //::AllocConsole();
    //::_wfreopen( L"CON", L"w", stdout );

    if ( nullptr == pCmdLine ) {
        puts( "[WinMain] pCmdLine: (null)" );
    }
    else if ( L'\0' == *pCmdLine ) {
        puts( "[WinMain] pCmdLine: '\\0'" );
    }
    else {
        printf( "[WinMain] pCmdLine: %S\n", pCmdLine );
    }

    printf( "[WinMain] GetCommandLine(): %S\n", ::GetCommandLineW() );
    
    //::MessageBoxW( nullptr, L"SlaveGui", L"bye", MB_OK );
    int      argc  = 0;
    wchar_t** pArgV = ::CommandLineToArgvW( ::GetCommandLineW(), &argc );
    if ( nullptr != pArgV ) {
        fakeMain( argc, pArgV );
        ::LocalFree( pArgV );
    }

    return 0;
}