#include <Common/Config/Config.hpp>

#include <strsafe.h>

#include <thread>
#include <shellapi.h>

using namespace std;

void printChildMsg( DWORD pid, HANDLE hReader )
{
    while ( true ) {
        DWORD   readLen = 0;
        char buf[ MAX_PATH ];
        if ( ::ReadFile( hReader, buf, MAX_PATH, &readLen, nullptr ) ) {
            std::string msg( buf, readLen );
            printf( "%s", msg.c_str() );
        }
    }
}

bool createProcess( const wchar_t* pAppName, wchar_t* pCmdLine )
{
    ::SECURITY_ATTRIBUTES sa{ sizeof( ::SECURITY_ATTRIBUTES ), nullptr, TRUE };
    HANDLE hChildStdOutReader = NULL;
    HANDLE hChildStdOut = NULL;
    if ( FALSE == ::CreatePipe( &hChildStdOutReader, &hChildStdOut, &sa, 0 ) ) {
        return false;
    }
    ::SetHandleInformation( hChildStdOutReader, HANDLE_FLAG_INHERIT, 0 );

    ::STARTUPINFOW      startup{ sizeof( ::STARTUPINFOW ) };
    startup.dwFlags    = STARTF_USESTDHANDLES;
    startup.hStdOutput = hChildStdOut;
    startup.hStdError  = hChildStdOut;

    ::PROCESS_INFORMATION   procInfo{ 0 };

    BOOL bRet = ::CreateProcessW(
        pAppName,
        pCmdLine,
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        nullptr,
        &startup,
        &procInfo );
    if ( FALSE != bRet ) {
        printf( "CreateProcess( %S, %S ) -> PID %d\n", pAppName, pCmdLine, procInfo.dwProcessId );
        std::thread t( printChildMsg, procInfo.dwProcessId, hChildStdOutReader );

        ::WaitForSingleObject( procInfo.hProcess, INFINITE );
        ::TerminateThread( t.native_handle(), 1 );
        t.detach();
        ::CloseHandle( procInfo.hThread );
        ::CloseHandle( procInfo.hProcess );
        puts( "---------------------------------------------------" );
        return true;
    }

    printf( "CreateProcess() failed: 0x%08x\n", ::GetLastError() );
    return false;
}

void createProcessHybrid( const wchar_t* pAppName, const wchar_t* pCmdLine )
{
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //
    // CreateProcess( pAppName, pCmdLine );
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    wchar_t argList[ MAX_PATH ];
    ::StringCbPrintfW( argList, MAX_PATH, L"%s", pCmdLine );
    createProcess( pAppName, argList );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //
    // CreateProcess( nullptr, pAppName + pCmdLine );
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    wchar_t appAndArgList[ MAX_PATH ];
    ::StringCbPrintfW( appAndArgList, MAX_PATH, L"%s %s", pAppName, pCmdLine );
    createProcess( nullptr, appAndArgList );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //
    // CreateProcess( pAppName, pAppName + pCmdLine );
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ::StringCbPrintfW( appAndArgList, MAX_PATH, L"%s %s", pAppName, pCmdLine );
    createProcess( pAppName, appAndArgList );
}

int wmain( int argc, wchar_t* argv[] )
{
    wchar_t cli[] = L"SlaveCli.exe";
    //createProcess( L"SlaveCli.exe", nullptr );
    //createProcess( nullptr, cli );
    createProcessHybrid( L"SlaveCli.exe", L"arg1" );
    //createProcessHybrid( L"SlaveCli.exe", L"arg1 arg2" );

    wchar_t gui[] = L"SlaveGui.exe";
    //createProcess( L"SlaveGui.exe", nullptr );
    //createProcess( nullptr, gui );
    createProcessHybrid( L"SlaveGui.exe", L"arg1" );
    //createProcessHybrid( L"SlaveGui.exe", L"arg1 arg2" );

    return 0;
}