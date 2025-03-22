#pragma once

#include "il2cpp_header.h"

#include <cstdint>
#include <string>

namespace dumper
{
    enum dump_status : int8_t
    {
        NONE = -1,
        SUCCESS = 0,
        ERRORR = 1,
        ERROR_SYMBOLS = 2,
        ERROR_FRAMEWORK = 3
    };
    
    extern dump_status status;
    extern Il2CppDomain* domain;
    extern std::string dump_dir;

    dump_status dump( );
}