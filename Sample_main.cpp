#include <tchar.h>
#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include "AsrPolychromeSDKDefine.h"

typedef DWORD (WINAPI *MCISENDSTRINGA)(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
typedef DWORD (WINAPI *MCISENDSTRINGW)(LPCWSTR lpstrCommand, LPWSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
#if defined(UNICODE) || defined(_UNICODE)
typedef MCISENDSTRINGW MCISENDSTRING;
#else
typedef MCISENDSTRINGA MCISENDSTRING;
#endif

#if (!defined(_WIN64) | defined(_M_X86))
#define DLL_FILE_NAME    _T("AsrPolychromeSDK32.dll")
typedef std::wstring StdString;
#else
#define DLL_FILE_NAME    _T("AsrPolychromeSDK64.dll")
typedef std::string StdString;
#endif

HMODULE hDLL = NULL;

typedef DWORD(__cdecl *POLYCHROME_SDKINIT)();
typedef DWORD(__cdecl *POLYCHROME_SDKRELEASE)();
typedef DWORD(__cdecl *POLYCHROME_GETLEDCONTROLLERCOUNT)(DWORD *Count);
typedef DWORD(__cdecl *POLYCHROME_GETLEDCONTROLLERINFO)(ASRLIB_ControllerInfo* Info);
typedef DWORD(__cdecl *POLYCHROME_GETLEDPATTERN)(unsigned char ChannelId, ASRLIB_LedPattern* LedPattern);
typedef DWORD(__cdecl *POLYCHROME_SETLEDPATTERN)(unsigned char ChannelId, ASRLIB_LedPattern* LedPattern);
typedef DWORD(__cdecl *POLYCHROME_SETLEDCOLORCONFIG)(unsigned char ChannelId, ASRLIB_LedColor* LedColor, UINT16 LedSize, unsigned char Brightness);
typedef DWORD(__cdecl *POLYCHROME_SETLEDCOLORS)();
typedef DWORD(__cdecl *POLYCHROME_SETLEDCHANNELCONFIG)(unsigned char ChannelId, ASRLIB_ChannelConfig* Config);
typedef DWORD(__cdecl *POLYCHROME_BACKTODEFAULT)();
typedef DWORD(__cdecl *POLYCHROME_SAVEUSERDATA)();

POLYCHROME_SDKINIT Polychrome_SDKInit = NULL;
POLYCHROME_SDKRELEASE Polychrome_SDKRelease = NULL;
POLYCHROME_GETLEDCONTROLLERCOUNT Polychrome_GetLedControllerCount = NULL;
POLYCHROME_GETLEDCONTROLLERINFO Polychrome_GetLedControllerInfo = NULL;
POLYCHROME_GETLEDPATTERN Polychrome_GetLedPattern = NULL;
POLYCHROME_SETLEDPATTERN Polychrome_SetLedPattern = NULL;
POLYCHROME_SETLEDCOLORCONFIG Polychrome_SetLedColorConfig = NULL;
POLYCHROME_SETLEDCOLORS Polychrome_SetLedColors = NULL;
POLYCHROME_SETLEDCHANNELCONFIG Polychrome_SetLedChannelConfig = NULL;
POLYCHROME_BACKTODEFAULT Polychrome_BackToDefault = NULL;
POLYCHROME_SAVEUSERDATA Polychrome_SaveUserData = NULL;

bool InitLibrary();
bool ReleaseLibrary(); 

/* 
    --------------------
    Channel ChannelId   
    --------------------
     12V_1      0       
     12V_2      1       
     5V_1       2        
     5V_2       3
     PCH        4
     IO Cover   5
     Side       6 
     Reserved   7     
*/
void LedPattern_Off(int ChannelId, bool ApplyAll = false);
void LedPattern_Static(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, bool ApplyAll = false);
void LedPattern_Breathing(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Strobe(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Cycling(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Random(int ChannelId, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Music();
void LedPattern_Wave(int ChannelId, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Spring(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Stack(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Cram(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Scan(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Neon(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Water(int ChannelId, unsigned char Speed, bool ApplyAll = false);
void LedPattern_Rainbow(int ChannelId, unsigned char Speed, bool ApplyAll = false);

ASRLIB_ControllerInfo ControllerInfo;
ASRLIB_LedColor CHColors[ASR_LED_CH_MAXNUM];

#define		BREATH_STEP_DIVIDER		32
#define		LIGHT_ON_DELAY			200			//0.2sec
#define		LIGHT_OFF_DELAY			400			//0.4sec
#define		BREATH_LIGHT_UP			0
#define		BREATH_FADE_DOWN		1
#define		SYNC_DELAY_COUNT		20000
#define		SYNC_BREATHING_DELAY  	20

#define		VGA_TEST_DELAY  	500

typedef struct _BrPARMStruc {
	unsigned char StepCount;
	unsigned char BreathInOut; // 0 = light up, 1 = fade down
	unsigned char BreathDelay;
}BrPARMStruc;

ASRLIB_LedColor BrRGB, tBrRGB;
BrPARMStruc BrPARM;

void BreathingCtrl();

int _tmain(int argc, TCHAR** argv)
{
	DWORD dwControllers;
	
	if (!InitLibrary()) {
		_tprintf(_T("Initialize library fail!\n"));
		return 1;
	}

	if (Polychrome_SDKInit() != ASRLIB_SUCCESS) {
		_tprintf(_T("Asr SDK Init Fail\n"));
		return 1;
	}

	if (Polychrome_GetLedControllerCount(&dwControllers) == ASRLIB_SUCCESS) {
		_tprintf(_T("RGB Controller Count = %d\n"), dwControllers);

		if (dwControllers == 0) {
			return 1;
		}
	}

	//ASRLIB_ControllerInfo ControllerInfo;
	if (Polychrome_GetLedControllerInfo(&ControllerInfo) == ASRLIB_SUCCESS) {
		printf("------------- Controller %d Information -------------\n", ControllerInfo.ControllerId);
		printf("   Type            : %d\n", ControllerInfo.Type);
		printf("   Firmware Version: %08X\n", ControllerInfo.FirmwareVersion);
		printf("   Firmware Date   : %08X\n", ControllerInfo.FirmwareDate);
		printf("   MAX. Channels   : %d\n", ControllerInfo.MaxLedChannels);
		printf("   Support Channel : ");
		for (int n = 0; n < ControllerInfo.MaxLedChannels; n++) {
			printf("CH%d[%s] ", n, (ControllerInfo.ActiveChannel >> n) & 0x01 ? "v" : " ");
		}
		printf("\n");
	}

	ASRLIB_LedPattern LedPattern;
	for (int n = 0; n < ControllerInfo.MaxLedChannels; n++) {
		if ((ControllerInfo.ActiveChannel >> n) & 0x01) {
			Polychrome_GetLedPattern(n, &LedPattern);
			printf("Channle[%d] Pattern[%d] Color[R:%d, G:%d, B:%d] Speed[%d] ApplyAll[%s] MAXLeds[%03d] RGSwap[%s]\n",
				n, LedPattern.PatternId, LedPattern.ColorR, LedPattern.ColorG, LedPattern.ColorB, LedPattern.Speed, LedPattern.ApplyAll ? "true" : "false",
				ControllerInfo.ChannelConfig[n].MaxLeds, ControllerInfo.ChannelConfig[n].RGSwap ? "true" : "false");
		}
	}

	system("PAUSE");	

	printf("------------- The breathing mode is realized by controlling each LED on the MB.\n");
	printf("------------- Demo Starts -------------\n");
	BrRGB.ColorR = 0xFF;
	BrRGB.ColorG = 0x00;
	BrRGB.ColorB = 0x00;
	memset(CHColors, 0, sizeof(CHColors));

	UCHAR TestStep = 0;
	while (1) {
		BreathingCtrl();

		TestStep++;
		if (TestStep == 255) break;
	}

	printf("------------- Demo end    -------------\n");
	printf("------------- Pattern back to default -\n");

	Polychrome_BackToDefault();
	system("PAUSE");

	Polychrome_SaveUserData();

    ReleaseLibrary();

    return 0;
	
}

bool InitLibrary()
{
	hDLL = LoadLibrary(DLL_FILE_NAME);
	if (hDLL == NULL) {
		return false;
	}

	if (!Polychrome_SDKInit) {
		Polychrome_SDKInit = (POLYCHROME_SDKINIT)GetProcAddress(hDLL, "Polychrome_SDKInit");
	}

	if (!Polychrome_SDKRelease) {
		Polychrome_SDKRelease = (POLYCHROME_SDKRELEASE)GetProcAddress(hDLL, "Polychrome_SDKRelease");
	}

	if (!Polychrome_GetLedControllerCount) {
		Polychrome_GetLedControllerCount = (POLYCHROME_GETLEDCONTROLLERCOUNT)GetProcAddress(hDLL, "Polychrome_GetLedControllerCount");
	}

	if (!Polychrome_GetLedControllerInfo) {
		Polychrome_GetLedControllerInfo = (POLYCHROME_GETLEDCONTROLLERINFO)GetProcAddress(hDLL, "Polychrome_GetLedControllerInfo");
	}

	if (!Polychrome_GetLedPattern) {
		Polychrome_GetLedPattern = (POLYCHROME_GETLEDPATTERN)GetProcAddress(hDLL, "Polychrome_GetLedPattern");
	}

	if (!Polychrome_SetLedPattern) {
		Polychrome_SetLedPattern = (POLYCHROME_SETLEDPATTERN)GetProcAddress(hDLL, "Polychrome_SetLedPattern");
	}

	if (!Polychrome_SetLedColorConfig) {
		Polychrome_SetLedColorConfig = (POLYCHROME_SETLEDCOLORCONFIG)GetProcAddress(hDLL, "Polychrome_SetLedColorConfig");
	}

	if (!Polychrome_SetLedColors) {
		Polychrome_SetLedColors = (POLYCHROME_SETLEDCOLORS)GetProcAddress(hDLL, "Polychrome_SetLedColors");
	}

	if (!Polychrome_SetLedChannelConfig) {
		Polychrome_SetLedChannelConfig = (POLYCHROME_SETLEDCHANNELCONFIG)GetProcAddress(hDLL, "Polychrome_SetLedChannelConfig");
	}

	if (!Polychrome_BackToDefault) {
		Polychrome_BackToDefault = (POLYCHROME_BACKTODEFAULT)GetProcAddress(hDLL, "Polychrome_BackToDefault");
	}

	if (!Polychrome_SaveUserData) {
		Polychrome_SaveUserData = (POLYCHROME_SAVEUSERDATA)GetProcAddress(hDLL, "Polychrome_SaveUserData");
	}

	return true;
}

bool ReleaseLibrary()
{   
    // Release SDK
    Polychrome_SDKRelease();

	if (hDLL != NULL) {
		FreeLibrary(hDLL);
	}

	return true;
}

void BreathingCtrl()
{
	Sleep(SYNC_BREATHING_DELAY);

	tBrRGB.ColorR = (BrRGB.ColorR / BREATH_STEP_DIVIDER) * BrPARM.StepCount;
	tBrRGB.ColorG = (BrRGB.ColorG / BREATH_STEP_DIVIDER) * BrPARM.StepCount;
	tBrRGB.ColorB = (BrRGB.ColorB / BREATH_STEP_DIVIDER) * BrPARM.StepCount;

	for (int i = 0; i < ASR_LED_CH_COUNT; i++) {
		for (int j = 0; j < ControllerInfo.ChannelConfig[i].MaxLeds; j++) {
			CHColors[j].ColorR = tBrRGB.ColorR;
			CHColors[j].ColorG = tBrRGB.ColorG;
			CHColors[j].ColorB = tBrRGB.ColorB;
		}
		Polychrome_SetLedColorConfig(i, CHColors, ControllerInfo.ChannelConfig[i].MaxLeds, ASR_LED_BRIGHTNESS_MAX);
	}

	Polychrome_SetLedColors();

	if (BrPARM.BreathInOut == BREATH_LIGHT_UP) {
		if (BrPARM.StepCount == BREATH_STEP_DIVIDER) {
			BrPARM.BreathInOut = BREATH_FADE_DOWN; // change to fade down
												   // Sleep(LIGHT_ON_DELAY);
		}
		else {
			BrPARM.StepCount++;
		}
	}
	else { // BREATH_FADE_DOWN
		if (BrPARM.StepCount == 0) {
			BrPARM.BreathInOut = BREATH_LIGHT_UP; // change to light up
			Sleep(LIGHT_OFF_DELAY);
		}
		else {
			BrPARM.StepCount--;
		}
	}
}

void LedPattern_Off(int ChannelId, bool ApplyAll)
{    
    int PatternId = ASR_LED_OFF;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;    
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Static(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, bool ApplyAll)
{
    int PatternId = ASR_LED_STATIC;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Breathing(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_BREATHING;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
	Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Strobe(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_STROBE;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Cycling(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_CYCLING;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Random(int ChannelId, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_RANDOM;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Music()
{
    int PatternId = ASR_LED_MUSIC;
    int ChannelId = 0x00;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;    
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Wave(int ChannelId, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_WAVE;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Spring(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_SPRING;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Stack(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_STACK;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Cram(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_CRAM;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Scan(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_SCAN;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Neon(int ChannelId, unsigned char ColorR, unsigned char ColorG, unsigned char ColorB, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_NEON;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.ColorR = ColorR;
    LedPattern.ColorG = ColorG;
    LedPattern.ColorB = ColorB;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Water(int ChannelId, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_WATER;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}

void LedPattern_Rainbow(int ChannelId, unsigned char Speed, bool ApplyAll)
{
    int PatternId = ASR_LED_RAINBOW;
    ASRLIB_LedPattern LedPattern;
    memset(&LedPattern, 0, sizeof(ASRLIB_LedPattern));
	Polychrome_GetLedPattern(ChannelId, &LedPattern);
    LedPattern.PatternId = PatternId;    
    LedPattern.Speed = Speed;
    LedPattern.ApplyAll = ApplyAll;
    Polychrome_SetLedPattern(ChannelId, &LedPattern);
}