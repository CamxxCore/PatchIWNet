#pragma once

namespace Utility {

void WriteFloat( LPVOID address, float value );

void WriteMemory( LPVOID address, const char * bytes, int len );

void WriteNop( LPVOID address, int len );
}

