#include "stdafx.h"

void Utility::WriteFloat( LPVOID address, const float value ) {

    DWORD oProtect;
    VirtualProtect( address, 4, PAGE_EXECUTE_READWRITE, &oProtect );

    *reinterpret_cast<float*>( address ) = value;

    VirtualProtect( address, 4, oProtect, nullptr );
}

void Utility::WriteMemory( LPVOID address, const char* bytes, const int len ) {

    DWORD oProtect;
    VirtualProtect( address, len, PAGE_EXECUTE_READWRITE, &oProtect );

    memcpy( reinterpret_cast<LPVOID>( address ), bytes, len );

    VirtualProtect( address, len, oProtect, nullptr );
}

void Utility::WriteNop( LPVOID address, const int len ) {

    DWORD oProtect;
    VirtualProtect( address, len, PAGE_EXECUTE_READWRITE, &oProtect );

    memset( address, 144, len );

    VirtualProtect( address, len, oProtect, nullptr );
}
