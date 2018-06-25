#pragma once
#include <cstdint>


union color_s {
    color_s( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    color_s( uint8_t r, uint8_t g, uint8_t b ) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }

    color_s( color_s color, uint8_t a ) {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
        this->a = a;
    }

    color_s( uint32_t rgba ) {
        this->rgba = rgba;
    }

    color_s() {}

    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    static color_s red() {
        return { 255, 0, 0 };
    }

    static color_s green() {
        return { 0, 255, 0 };
    }

    static color_s blue() {
        return { 0, 0, 255 };
    }

    static color_s white() {
        return { 255, 255, 255 };
    }

    static color_s black() {
        return { 0, 0, 0 };
    }

    static color_s empty() {
        return { 0, 0, 0, 0 };
    }

    uint32_t rgba;
};

/* 3260 */
struct scr_localVar_t {
    unsigned int name;
    unsigned int sourcePos;
};

/* 3261 */
struct scr_block_s {
    int abortLevel;
    int localVarsCreateCount;
    int localVarsPublicCount;
    int localVarsCount;
    char localVarsInitBits[8];
    scr_localVar_t localVars[64];
};

union sval_u {
    char type;
    unsigned int stringValue;
    unsigned int idValue;
    float floatValue;
    int intValue;
    sval_u *node;
    unsigned int sourcePosValue;
    const char *codePosValue;
    const char *debugString;
    scr_block_s *block;
};

/* 2071 */
struct iobuf {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfnam;
};

/* 3258 */
struct yy_buffer_state {
    iobuf *yy_input_file;
    char *yy_ch_buf;
    char *yy_buf_pos;
    unsigned int yy_buf_size;
    int yy_n_chars;
    int yy_is_our_buffer;
    int yy_is_interactive;
    int yy_at_bol;
    int yy_fill_buffer;
    int yy_buffer_status;
};

typedef enum {
    NA_BOT,
    NA_BAD, // an address lookup failed
    NA_LOOPBACK,
    NA_BROADCAST,
    NA_IP,
    NA_IPX,
    NA_BROADCAST_IPX
} netadrtype_t;

enum netsrc_t {
    NS_CLIENT1 = 0,
    NS_MAXCLIENTS = 1,
    NS_SERVER = 2,
    NS_PACKET = 3,
    NS_INVALID_NETSRC = 4
};

typedef struct netadr_s {
    netadrtype_t	type;

    BYTE	ip[4];

    unsigned short	port;

    BYTE	ipx[10];
} netadr_t;

#pragma pack(push, 1)
typedef struct client_s {
    // 0
    int state;
    // 4
    char pad[36];
    // 40
    netadr_s adr;
    // 60
    char pad1[1568];
    // 1628
    char connectInfoString[1024];
    // 2652
    char pad2[133192];
    // 135844
    char name[16];
    // 135860
    char pad3[12];
    // 135872
    int snapNum;
    // 135876
    int pad4;
    // 135880
    short ping;
    // 135882
    //char pad5[142390];
    char pad5[133158];
    // 269040
    int isBot;
    // 269044
    char pad6[9228];
    // 278272
    __int64 steamid;
    // 278280
    char pad7[403592];
} client_t;
#pragma pack(pop)

typedef struct IWSteamClient {
    char pad0[0x90];
    unsigned long long steamID;
    unsigned long long steamID_1;
} IWSteamClient_s;

typedef struct PartyData_t {
    const char* type; //0x0-0x4
    int unk; //0x4-0x8
    bool bMigrationEnabled; //0x8-0x9
    bool unk1; //0x9-0xA
    char pad0[0x17D4]; //0xA-0X17FC
    bool bHostMigrate;
    //char pad1[0xE];
    //int iwSteamState; //0x17FC-0x17DE
    char pad1[0x9]; //0x17DE-0x17E7
    unsigned long long steamID; //0x17E8-0x17F0
    char pad2[0x58]; //0x17F0-0x1848
    int partyID; //0x1848-0x184C
    char pad3[0x4]; //0x184C-0x1850
    int mappack_flags; //0x1850-0x1854
    char pad4[0x18]; //0x1854-0x185C
    PartyData_t * partyToNotify; //0x186C-0x1870
} PartyData_s;