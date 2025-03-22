#include "normalize.hpp"

#include <unordered_map>

namespace normal
{
	std::string str( std::string def )
	{
        static std::unordered_map<std::string, std::string> types
        {
            {"Boolean", "bool"},
            {"Boolean[]", "bool[]"},
            {"Single", "float"},
            {"Single[]", "float[]"},
            {"Int32", "int"},
            {"Int32[]", "int[]"},
            {"String", "string"},
            {"String[]", "string[]"},
            {"Void", "void"},
            {"Object", "object"},
            {"Object[]", "object[]"}
        };
        auto it = types.find( def );
        if ( it != types.end( ) )
            return it->second;
        return def;
	}
}