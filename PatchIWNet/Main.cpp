#include "stdafx.h"
#include <MinHook.h>
#include "ConfigManager.h"

using namespace Utility;

PartyData_s * g_lobbyData, * g_partyData;

IWSteamClient * g_steamClient;

const int MIN_GAME_VER = 208;

uint64_t qwSteamID = 0;

typedef bool( __cdecl *Steam_IsInValidLobby_t )( int, int );
Steam_IsInValidLobby_t oIsInValidLobby;

bool Steam_IsInValidLobby_Hook( int unk, int unk1 ) {

    return true;
}

using PartyHost_Frame_t = int( __cdecl* )( PartyData_s*, int, int );
PartyHost_Frame_t oPartyHostFrame;

int PartyHost_Frame_Hook( PartyData_s * party, const int localControllerIndex, const int localClientNum ) {

    g_lobbyData->steamID = g_partyData->steamID = g_steamClient->steamID = g_steamClient->steamID_1 = qwSteamID;

    return oPartyHostFrame( party, localControllerIndex, localClientNum );
}

typedef int( __cdecl *Menu_Paint_t )();
Menu_Paint_t oMenuPaint;

bool connectToOthers = false;

int Menu_Paint_Hook() {

    if ( GetAsyncKeyState( VK_F10 ) & 1 ) {

        if ( connectToOthers ) {

            WriteMemory( (LPVOID)0x4D6DB9, "\x0F\x85\x01\x00", 4 ); //connect to others
        }

        else {

            WriteMemory( (LPVOID)0x4D6DB9, "\xE9\xB7\x01\x00", 4 ); //dont connect to others
        }

        Beep( connectToOthers ? 750 : 900, 300 );

        connectToOthers = !connectToOthers;
    }

    return oMenuPaint();
}

int UnprotectPE() {

    // unprotect the entire PE
    HMODULE hModule = GetModuleHandle( nullptr );

    if ( !hModule ) {

        return ERROR_MOD_NOT_FOUND;
    }

    const auto header = reinterpret_cast<PIMAGE_DOS_HEADER>( hModule );

    const auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<uintptr_t>( hModule ) + header->e_lfanew );

    const SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;

    DWORD oProtect;

    VirtualProtect( static_cast<LPVOID>( hModule ), size, PAGE_EXECUTE_READWRITE, &oProtect );

    return ERROR_SUCCESS;
}

int InitializeHooks() {

    if ( MH_Initialize() != MH_OK )
        return 1;

    if ( CreateHook( reinterpret_cast<LPVOID>( 0x628630 ),
                     reinterpret_cast<LPVOID>( Steam_IsInValidLobby_Hook ), &oIsInValidLobby ) )
        return 1;

    if ( CreateHook( reinterpret_cast<LPVOID>( 0x4DA260 ),
                     reinterpret_cast<LPVOID>( PartyHost_Frame_Hook ), &oPartyHostFrame ) )
        return 1;

    if ( CreateHook( reinterpret_cast<LPVOID>( 0x4F4A70 ),
                     reinterpret_cast<LPVOID>( Menu_Paint_Hook ), &oMenuPaint ) )
        return 1;

    MH_EnableHook( nullptr );

    return ERROR_SUCCESS;
}

bool main_init() {

    qwSteamID = ConfigManager::GetUInt64( "Lobby", "SteamID", 0 );

    LOG( "PatchIWNet: Steam Lobby ID - '%llu'", qwSteamID );

    auto result = BytePattern( (PBYTE)"\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x10\xB8", "x????x????x????x????xxxx" );

    if ( result.size() <= 0 ) {

        MessageBoxA( nullptr, "PatchIWNet: Game version wasn't retrieved. Exiting...", "Error", MB_OK );
        return false;
    }

    const auto lpstr = *reinterpret_cast<const char**>( result.get().get( 11 ) );

    LOG( "Found string (%p)", lpstr );

    char buffer[4];
    memcpy_s( buffer, 4, lpstr, 3 );
    buffer[3] = '\0';

    const int nGameVersion = std::stol( buffer );

    if ( nGameVersion < MIN_GAME_VER ) {
badVersion:
        MessageBox( nullptr, _T( "PatchIWNet: Unsupported game version. Exiting..." ), _T( "Error" ), MB_OK );

        return false;
    }

	if ( UnprotectPE() != ERROR_SUCCESS ) {
		return false;
	}

    const char * versionText = "%s.%i (VAC disabled)";

    switch ( nGameVersion ) {
    case 208: {

        g_lobbyData = (PartyData_s*)0x1088E50;

        g_partyData = (PartyData_s*)0x1085688;

        g_steamClient = *(IWSteamClient**)0x672A90C;

        //*(BYTE*)0x5B5A92 = 0xEB;

        // currentnetcode (test)
        *(DWORD*)0x4F3021 = 148;

        // bypass callback from IWSteamClient::OnLobbyCreated
        *(BYTE*)0x4DBFFD = 0x74;

        // bypass callback from IWSteamClient::JoinLobby
        *(BYTE*)0x4D3340 = 0x75;

        // steam check that is called each frame
        WriteNop( (LPVOID)0x49D99F, 5 );

        strcpy( (char*)0x79122C, versionText );

        *(int32_t*)0x4D8A8B = 0x79122C;
    }
    break;
    case 211: {

        g_lobbyData = (PartyData_s*)0x10F9268;

        g_partyData = (PartyData_s*)0x10F5A98;

        g_steamClient = *(IWSteamClient**)0x66547BC;

        WriteNop( (LPVOID)0x4DB423, 4 );

        WriteNop( (LPVOID)0x4921AE, 5 );

        // bypass callback from IWSteamClient::OnLobbyCreated
        WriteNop( (LPVOID)0x628A7B, 2 );

        // bypass callback from IWSteamClient::JoinLobby
        *(BYTE*)0x628B20 = 0x75;

        // jnz = jz in PartyHost_HandleClientHandshake (Prevent users from breaking matchmaking with their invalid lobbies)
        *(BYTE*)0x4DB7A1 = 0x74;

        // check in PartyHost_Frame iwSteamClient->steamID == g_lobbyData->steamID
        WriteNop( (LPVOID)0x4DA28D, 22 );

        // steam check that is called each frame
        WriteNop( (LPVOID)0x628FCF,  5 );

        strcpy( (char*)0x6F1AAC, versionText );

        *(int32_t*)0x59B25B = 0x6F1AAC;

        if ( InitializeHooks() != ERROR_SUCCESS )
            return false;
    }
    break;
    default:
        goto badVersion;
    }

    return true;
}

void unload() {

    MH_Uninitialize();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     ) {
    switch ( ul_reason_for_call ) {
    case DLL_PROCESS_ATTACH:
        main_init();
        break;
    case DLL_PROCESS_DETACH:
        unload();
    }
    return TRUE;
}
