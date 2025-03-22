#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <utility>
#include <sstream>



class File {
public:
    FILE* file;

    File( ) : file( nullptr ) {
    }
    File( const std::string& path, const char* mode ) {
        open( path, mode );
    }
    ~File( ) {
        if ( file ) {
            fclose( file );
        }
    }
    inline int open( const std::string& path, const char* mode ) {
        
        return fopen_s(&file, path.c_str( ), mode );
    }
    inline void close( ) {
        if ( file ) {
            fclose( file );
            file = nullptr;
        }
    }
    inline void write( const char* data ) {
        fwrite( data, 1, std::strlen( data ), file );
    }
    inline void write( std::string& data ) {
        fwrite( data.c_str( ), 1, data.size( ), file );
    }
    inline void write( std::stringstream& data ) {
        fwrite( data.str( ).c_str( ), 1, data.str( ).size( ), file );
    }

    bool ok( ) const {
        return file != nullptr;
    }
    operator FILE* ( ) {
        return file;
    }
};