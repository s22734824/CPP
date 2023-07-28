#pragma once

#include <tchar.h>>
#include <Windows.h>
#include "AsrPolychromeSDKDefine.h"

ASREXPORT DWORD ASRAPI Polychrome_SDKInit();
ASREXPORT DWORD ASRAPI Polychrome_SDKRelease();

ASREXPORT DWORD ASRAPI Polychrome_GetLedControllerCount(DWORD *Count);
ASREXPORT DWORD ASRAPI Polychrome_GetLedControllerInfo(ASRLIB_ControllerInfo* Info);

ASREXPORT DWORD ASRAPI Polychrome_GetLedPattern(unsigned char ChannelId, ASRLIB_LedPattern* LedPattern);
ASREXPORT DWORD ASRAPI Polychrome_SetLedPattern(unsigned char ChannelId, ASRLIB_LedPattern* LedPattern);

ASREXPORT DWORD ASRAPI Polychrome_SetLedColorConfig(unsigned char ChannelId, ASRLIB_LedColor* LedColor, UINT16 LedSize, unsigned char Brightness);
ASREXPORT DWORD ASRAPI Polychrome_SetLedColors();

ASREXPORT DWORD ASRAPI Polychrome_SetLedChannelConfig(unsigned char ChannelId, ASRLIB_ChannelConfig* Config);

ASREXPORT DWORD ASRAPI Polychrome_BackToDefault();
ASREXPORT DWORD ASRAPI Polychrome_SaveUserData();