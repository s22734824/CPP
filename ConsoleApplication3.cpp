// ConsoleTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
//#include <D:\Project\Local\CppDllTest\ConsoleTest\ConsoleTest\DllTest.dll>

// If you don't have the DLL header file, manually define the macros
#define ASR_EXPORT __declspec(dllexport)
#define ASR_API __cdecl

typedef enum
{
	ESCORE_HWM_CPU_TEMP,
	ESCORE_HWM_MB_TEMP,
	ESCORE_HWM_CPU_FAN1_SPEED,
	ESCORE_HWM_CPU_FAN2_SPEED,
	ESCORE_HWM_CHASSIS_FAN1_SPEED,
	ESCORE_HWM_CHASSIS_FAN2_SPEED,
	ESCORE_HWM_CHASSIS_FAN3_SPEED,
	ESCORE_HWM_CHASSIS_FAN4_SPEED,
	ESCORE_HWM_SB_FAN1_SPEED,
	ESCORE_HWM_POWER_FAN_SPEED,
	ESCORE_HWM_CPU_FAN12_SHARE_SPEED,
	ESCORE_HWM_CHASSIS_FAN12_SHARE_SPEED,
	ESCORE_HWM_VCOREV,
	ESCORE_HWM_5V,
	ESCORE_HWM_12V,
	ESCORE_HWM_3P3V,
	ESCORE_HWM_CPU_INPUTV,
	ESCORE_HWM_VIN,
	ESCORE_HWM_CARDSIDE_TEMP,
	ESCORE_HWM_FRNT_FAN1_SPEED,
	ESCORE_HWM_FRNT_FAN2_SPEED,
	ESCORE_HWM_FRNT_FAN3_SPEED,
	ESCORE_HWM_VDDCR_SOCV,
	ESCORE_HWM_DRAMV,
	ESCORE_HWM_VDD_MISCV,
	ESCORE_HWM_VCCIOV,
	ESCORE_HWM_VCCSAV,
	ESCORE_HWM_VCCSTSFRV,
	ESCORE_HWM_3VSBV,
	ESCORE_HWM_VBATV,
	ESCORE_HWM_TR1_TEMP,
	ESCORE_HWM_DCIN,
	ESCORE_HWM_GPU_FAN_SPEED,
	ESCORE_HWM_SYSTEM_FAN1_SPEED,
	ESCORE_HWM_3V,
	ESCORE_HWM_CPU_SPEED,
} ESCORE_HWM_ITEM;

extern "C" bool ASR_API AsrLibGetHardwareMonitor(ESCORE_HWM_ITEM Item, double* Value);
extern "C" bool ASR_API AsrLibDllInit();


int main()
{
	std::cout << "Hello World!\n";

	HMODULE dllHandle = LoadLibrary(L"C:\\Users\\iBUYPOWER\\Desktop\\AsrCoreAPI_Tuning_B760M_C_B760M_PRO_RS_20230717\\AsrCoreAPI_Tuning_B760M_C_B760M_PRO_RS\\AsrCore.dll");
	if (dllHandle != nullptr) {

		typedef bool(ASR_API* InitDll)();
		InitDll initDll = reinterpret_cast<InitDll>(GetProcAddress(dllHandle, "AsrLibDllInit"));

		bool res = initDll();

		// Step 2: Get the function address
		typedef bool(ASR_API* GetMonitorFunc)(ESCORE_HWM_ITEM Item, double* Value);
		GetMonitorFunc getHardwareMonitor = reinterpret_cast<GetMonitorFunc>(GetProcAddress(dllHandle, "AsrLibGetHardwareMonitor"));
		double value = -1;
		if (getHardwareMonitor != nullptr) {
			// Step 3: Call the function
			double* address = &value;
			bool success = getHardwareMonitor(ESCORE_HWM_ITEM::ESCORE_HWM_CPU_TEMP, &value);
			// Step 4: Print the result
			std::cout << "Result from Print: " << value << std::endl;
		}
		else {
			std::cout << "Failed to get the function address." << std::endl;
		}
		// Step 5: Unload the DLL
		FreeLibrary(dllHandle);
	}
	else {
		std::cout << "Failed to load the DLL." << std::endl;
	}

	return 0;

}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
