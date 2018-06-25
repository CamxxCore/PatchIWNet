#include "stdafx.h"

static char g_logBuffer[4000];

namespace Utility {
Logger::Logger() : path( GetModuleName( nullptr ) + ".log" )
{ }

void Logger::Write( const char * format, ... ) const {

    va_list vl;
    va_start( vl, format );
    vsprintf_s( g_logBuffer, format, vl );
    va_end( vl );

    std::ofstream ofs( path, std::ios::app );

    ofs << va("[%s] [LOG] %s\n", GetShortTimeString().c_str(), g_logBuffer);

    ofs.close();
}

void Logger::Remove() const {

    if ( !FileExists( path ) ) return;

    remove( path.c_str() );
}

Logger::~Logger() {

    Remove();
}
}
