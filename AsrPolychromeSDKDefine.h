#pragma once

#define ASRAPI                  __cdecl
#define ASREXPORT               extern "C" __declspec(dllexport)

#define ASRLIB_VER_MAJOR        1
#define ASRLIB_VER_MINOR        0
#define ASRLIB_VER_RELEASE      0
#define ASRLIB_VER_REVISION     0

#define ASR_ONBOARD_LED_VID		0x26CE
#define ASR_ONBOARD_LED_PID		0x01A2

#pragma pack(push, 1)

typedef enum _ASRLIB_ERRCODE
{
    ASRLIB_SUCCESS = 0,
    ASRLIB_FAILED = 0xFFFF,
    ASRLIB_LIB_NOT_INIT = 0x0100,
    ASRLIB_PLATFORM_UNSUPPORT = 0x0101,
    ASRLIB_API_UNSUPPORT = 0x0102,
    ASRLIB_API_NO_LONGER_AVAILABLE = 0x0103,
    ASRLIB_LIB_INIT_FAIL = 0x0104,
    ASRLIB_INVALID_PARAMETER = 0x0105,
    ASRLIB_INVALID_ID = 0x0106,
    ASRLIB_OUTBUF_RETURN_SIZE_INCORRECT = 0x0107,
    ASRLIB_ARRAY_LENGTH_INSUFFICIENT = 0x0108,
    ASRLIB_THREAD_LOCKED = 0x0109,
    ASRLIB_LIB_INVALID_VERSION = 0x010A,
	ASRLIB_ENUM_HIDDEV_FAIL = 0x010B,
} ASRLIB_ERRCODE;

typedef struct _ASRLIB_Version
{
    DWORD Major;
    DWORD Minor;
    DWORD Release;
    DWORD Revision;
} ASRLIB_Version;

typedef enum _ASRLIB_ControllerType
{
    RGB_CONTROLLER_MB = 0,
} ASRLIB_ControllerType;

typedef struct _ASRLIB_ChannelConfig
{
    unsigned int  MaxLeds;
    bool RGSwap;
} ASRLIB_ChannelConfig;

// --------- LED Channel Id ---------------------- Config -- MAX LEDs -- RGSwap ------
#define     ASR_LED_CH_12V1                 0x00    //         -           V
#define     ASR_LED_CH_12V2                 0x01    //         -           V
#define     ASR_LED_CH_5V1                  0x02    //         V           V
#define     ASR_LED_CH_5V2                  0x03    //         V           V
#define     ASR_LED_CH_PCH                  0x04    //         -           -
#define     ASR_LED_CH_IO_COVER             0x05    //         -           -
#define     ASR_LED_CH_SIDE                 0x06    //         -           -
#define     ASR_LED_CH_RESERVED             0x07    //         -           -

#define     ASR_LED_CH_COUNT                0x08

#define     ASR_LED_MAX_NUM				    322
#define     ASR_LED_CH_MAXNUM			    100
#define     ASR_LED_BRIGHTNESS_MAX			100

typedef struct _ASRLIB_ControllerInfo
{
    DWORD ControllerId;
    ASRLIB_ControllerType Type;  
    DWORD FirmwareVersion;
    DWORD FirmwareDate;
    DWORD MaxLedChannels;
    DWORD ActiveChannel;    
    ASRLIB_ChannelConfig ChannelConfig[ASR_LED_CH_COUNT];
} ASRLIB_ControllerInfo;

// If Parameter = -. This parameter has no function.

// --------- LED Pattern Id ------------------------ Parameters: ColorR -- ColorG -- ColorB -- Speed
#define     ASR_LED_OFF						0x00    //                                              
#define     ASR_LED_STATIC					0x01    //          V         V         V         -     
#define     ASR_LED_BREATHING				0x02    //          V         V         V         V     
#define     ASR_LED_STROBE                  0x03    //          V         V         V         V     
#define     ASR_LED_CYCLING                 0x04    //          V         V         V         V     
#define     ASR_LED_RANDOM                  0x05    //          -         -         -         V     
#define     ASR_LED_MUSIC                   0x06    //          -         -         -         -     
#define     ASR_LED_WAVE                    0x07    //          -         -         -         V     
#define     ASR_LED_SPRING                  0x08    //          V         V         V         V     
#define     ASR_LED_STACK                   0x09    //          V         V         V         V     
#define     ASR_LED_CRAM                    0x0A    //          V         V         V         V     
#define     ASR_LED_SCAN                    0x0B    //          V         V         V         V     
#define     ASR_LED_NEON                    0x0C    //          V         V         V         V     
#define     ASR_LED_WATER                   0x0D    //          -         -         -         V     
#define     ASR_LED_RAINBOW                 0x0E    //          -         -         -         V     

typedef struct _ASRLIB_LedPattern
{
    unsigned char PatternId;
    unsigned char ColorR;
    unsigned char ColorG;
    unsigned char ColorB;
    unsigned char Speed;    // 0 ~ 255 (Fast ~ Slow) Only for Asrock MB.
    bool ApplyAll;			// Only for Asrock MB.
} ASRLIB_LedPattern;

typedef struct _ASRLIB_LedColor
{
	unsigned char ColorR;
	unsigned char ColorG;
	unsigned char ColorB;
} ASRLIB_LedColor;

#pragma pack(pop)
