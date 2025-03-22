#include <Windows.h>
#include <iostream>

#include "il2cpp.hpp"
#include "dumper.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
    {
        AllocConsole( );
        FILE* file;
        freopen_s( &file, "CONOUT$", "w", stdout );

        SetConsoleTitle( "il2cpp runtime dumper by zxsrxt" );

        il2cpp::init( );
        if ( auto status = dumper::dump( ) != dumper::dump_status::SUCCESS )
        {
            std::cout << "Dump not succeed err: " << status << std::endl;
            return FALSE;
        }
        std::cout << "Dump successful // made by zxsrxt" << std::endl;
        std::cout << "Dump dir: " << dumper::dump_dir << "\\dump.cs" << std::endl;
    }
    return TRUE;
}

