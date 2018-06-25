#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>

namespace Utility {
HMODULE GetActiveModule();

std::string GetModuleName( HMODULE hModule );

std::string GetWorkingDirectory();

std::string GetFilenameWithoutPath( std::string& s, bool removeExtension = false );

std::vector<std::string> TokenizeString( std::string& s, char delimiter = ' ' );

std::string TrimString( std::string& s );

void CreateFolderTree( const std::string& s, char delimiter );

std::string VectorString( const float * vector, int size );

std::string HexString( BYTE * data, int len );

std::string GetShortTimeString();

std::string GetShortDateString();

bool FileExists( const std::string& fileName );

bool ModuleExists( const std::string& moduleName );

/**
* \brief char to long integer
*/
int ctol( char c );

bool PasteToClipboard( const std::string& text );

template<typename ... Args>
std::string va( const std::string& format, Args ... args ) {
    const size_t size = ::snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    std::unique_ptr<char[]> buf( new char[size] );
    snprintf( buf.get(), size, format.c_str(), args... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

template <typename T>
int CreateHook( LPVOID pTarget, LPVOID pDetour, T * result ) {
    if ( MH_CreateHook( pTarget, pDetour,
                        reinterpret_cast<LPVOID*>( result ) ) != MH_OK ) {
        return 1;
    }
    return 0;
}
}
