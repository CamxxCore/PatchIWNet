#pragma once

#define CLIENTT_ADDR 0x3172010
#define CLIENTT_SIZE 0xA6790
#define MAX_CLIENTS 18

static void Sys_Print( const char* msg ) {
    ( ( void( __cdecl* ) ( const char* ) ) 0x5C8AB0 )( msg );
}

static bool NET_CompareAdr( netadr_s *a, netadr_s *b ) {
    return ( ( bool( __cdecl* )( netadr_s*, netadr_s* ) ) 0x572E40 )( a, b );
}

static void SV_DropClient( client_t* client, const char *reason, bool tellThem ) {
    ( ( void( __cdecl* )( client_t*, const char*, bool ) ) 0x585CA0 )( client, reason, tellThem );
}

static void CBuf_AddText( char* text ) {
    ( ( void( __cdecl* )( int, char* ) ) 0x563BA0 )( 0, text );
}
