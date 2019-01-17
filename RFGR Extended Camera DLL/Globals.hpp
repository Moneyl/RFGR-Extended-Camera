#pragma once
#define PublicMode false
#define ColorLogMessages false
#define EnableSpectatorMode false 

/*RFGR Extended Camera DLL*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <utility>

#include <ctype.h>
#include <tlhelp32.h>
#include <math.h>

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <filesystem>
#include <experimental/filesystem>

#include "nlohmann/json.hpp"

//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")

//#include <dinput.h>

// Include GLM core features
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

// Include GLM extension
#include <glm/ext/matrix_transform.hpp>

using json = nlohmann::json;
namespace fs = std::experimental::filesystem;

/*
The initial values for these global vars are set in Globals.cpp
All of the ints representing keys are present in preparation for adding custom keybinds. However, keybinding is not yet implemented.
*/
extern float pi;

extern json MainConfig;
extern float DefaultFreeCameraSpeed;
extern float DefaultRotationSpeed;
extern bool UseDebugConsole;

extern int KeyDecreaseCameraSpeed;
extern int KeyIncreaseCameraSpeed;
extern int KeyToggleFreecamera;
extern int KeyToggleHud;
extern int KeyToggleFog;
extern int KeyCameraForward;
extern int KeyCameraBackward;
extern int KeyCameraLeft;
extern int KeyCameraRight;
extern int KeyCameraUp;
extern int KeyCameraDown;

extern int KeyDecreaseRotationSpeed;
extern int KeyIncreaseRotationSpeed;
extern int KeyToggleRotationControl;
extern int KeyResetRotation;
extern int KeyLockPitch;
extern int KeyLockYaw;
extern int KeyLockRoll;
extern int KeyDecreasePitch;
extern int KeyIncreasePitch;
extern int KeyDecreaseYaw;
extern int KeyIncreaseYaw;
extern int KeyDecreaseRoll;
extern int KeyIncreaseRoll;

extern HANDLE ConsoleHandle;

enum LogType
{
	LOGMESSAGE,
	LOGWARNING,
	LOGERROR,
	LOGSUCCESS
};

static WORD ConsoleMessageLabelTextAttributes = 0 | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
static WORD ConsoleMessageTextAttributes = 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static WORD ConsoleWarningTextAttributes = 0 | FOREGROUND_RED | FOREGROUND_GREEN;
static WORD ConsoleErrorTextAttributes = 0 | FOREGROUND_RED | FOREGROUND_INTENSITY;
static WORD ConsoleSuccessTextAttributes = 0 | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static WORD ConsoleDefaultTextAttributes = 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

static bool DirectoryExists(std::string Directory)
{
	if (!fs::is_directory(Directory) || !fs::exists(Directory))
	{
		return false;
	}
	else
	{
		return true;
	}
}

static void CreateDirectoryIfNull(std::string Directory)
{
	if (!DirectoryExists(Directory))
	{
		fs::create_directory(Directory);
	}
}

static float FloatConvertDegreesToRadians(float AngleInDegrees)
{
	return (pi / 180.0f) * AngleInDegrees;
}

static float sind(float AngleInDegrees)
{
	return sin(FloatConvertDegreesToRadians(AngleInDegrees));
}

static float cosd(float AngleInDegrees)
{
	return cos(FloatConvertDegreesToRadians(AngleInDegrees));
}

static float tand(float AngleInDegrees)
{
	return tan(FloatConvertDegreesToRadians(AngleInDegrees));
}

static HWND FindTopWindow(DWORD pid)
{
	std::pair<HWND, DWORD> params = { 0, pid };

	// Enumerate the windows using a lambda to process each window
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
	{
		auto pParams = (std::pair<HWND, DWORD>*)(lParam);

		DWORD processId;
		if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
		{
			SetLastError(-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;
	}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
	{
		return params.first;
	}

	return 0;
}

static DWORD GetProcessID(std::string ProcessName)
{
	PROCESSENTRY32 ProcessSnapshotStructure = { 0 };
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	ProcessSnapshotStructure.dwSize = sizeof(PROCESSENTRY32);

	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	if (Process32First(Snapshot, &ProcessSnapshotStructure) == FALSE)
	{
		return 0;
	}

	while (Process32Next(Snapshot, &ProcessSnapshotStructure))
	{
		if (!strcmp(ProcessSnapshotStructure.szExeFile, ProcessName.c_str()))
		{
			CloseHandle(Snapshot);
			return ProcessSnapshotStructure.th32ProcessID;
		}
	}
	CloseHandle(Snapshot);

	return 0;
}

static void EnforceFloatRange(float& Value, float Minimum, float Maximum)
{
	if (Value < Minimum)
	{
		Value = Minimum;
	}
	if (Value > Maximum)
	{
		Value = Maximum;
	}
}

static std::string GetTimeString(bool MilitaryTime)
{
	std::time_t t = std::time(0);
	std::tm now;
	localtime_s(&now, &t);

	std::string DateTime;
	std::string Year = std::to_string(now.tm_year + 1900);
	std::string Month = std::to_string(now.tm_mon + 1);
	std::string Day = std::to_string(now.tm_mday);
	std::string Hour;
	std::string Minutes = std::to_string(now.tm_min);

	if (Minutes.size() == 1)
	{
		Minutes.insert(0, 1, '0'); //Changes times like 1:6 to 1:06
	}

	if (MilitaryTime)
	{
		Hour = std::to_string(now.tm_hour);
	}
	else
	{
		if (now.tm_hour > 11) //PM
		{
			Hour = std::to_string(now.tm_hour - 12);
			DateTime += Hour + ":" + Minutes + " PM, ";
		}
		else //AM
		{
			if (now.tm_hour == 0)
			{
				Hour = "12";
				DateTime += Hour + ":" + Minutes + " AM, ";
			}
			else
			{
				Hour = std::to_string(now.tm_hour);
				DateTime += Hour + ":" + Minutes + " AM, ";
			}
		}
	}

	DateTime += Month + "/" + Day + "/" + Year;

	return DateTime;
}

static void SetConsoleAttributes(WORD Attributes)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

static void ResetConsoleAttributes()
{
	SetConsoleAttributes(ConsoleDefaultTextAttributes);
}

static void ConsoleLog(const char* Message, LogType Type, bool PrintTimeLabel = false, bool PrintTypeLabel = true, bool NewLine = false)
{
	if (Type == LOGMESSAGE)
	{
		if (PrintTypeLabel)
		{
			printf("[");
			SetConsoleAttributes(ConsoleMessageLabelTextAttributes);
			printf("+");
			ResetConsoleAttributes();
			if (PrintTimeLabel)
			{
				printf("]");
			}
			else
			{
				printf("] ");
			}
		}
		if (PrintTimeLabel)
		{
			printf("[");
			printf(GetTimeString(false).c_str());
			printf("] ");
		}
#if ColorLogMessages
		SetConsoleTextAttribute(ConsoleHandle, ConsoleMessageTextAttributes);
#endif
		printf(Message);
		if (NewLine)
		{
			printf("\n");
		}
	}
	else if (Type == LOGWARNING)
	{
		if (PrintTypeLabel)
		{
			printf("[");
			SetConsoleAttributes(ConsoleWarningTextAttributes);
			printf("Warning");
			ResetConsoleAttributes();
			if (PrintTimeLabel)
			{
				printf("]");
			}
			else
			{
				printf("] ");
			}
		}
		if (PrintTimeLabel)
		{
			printf("[");
			printf(GetTimeString(false).c_str());
			printf("] ");
		}
#if ColorLogMessages
		SetConsoleTextAttribute(ConsoleHandle, ConsoleWarningTextAttributes);
#endif
		printf(Message);
		if (NewLine)
		{
			printf("\n");
		}
	}
	else if (Type == LOGERROR)
	{
		if (PrintTypeLabel)
		{
			printf("[");
			SetConsoleAttributes(ConsoleErrorTextAttributes);
			printf("Error");
			ResetConsoleAttributes();
			if (PrintTimeLabel)
			{
				printf("]");
			}
			else
			{
				printf("] ");
			}
		}
		if (PrintTimeLabel)
		{
			printf("[");
			printf(GetTimeString(false).c_str());
			printf("] ");
		}
#if ColorLogMessages
		SetConsoleTextAttribute(ConsoleHandle, ConsoleErrorTextAttributes);
#endif
		printf(Message);
		if (NewLine)
		{
			printf("\n");
		}
	}
	else if (Type == LOGSUCCESS)
	{
		if (PrintTypeLabel)
		{
			printf("[");
			SetConsoleAttributes(ConsoleSuccessTextAttributes);
			printf("Success");
			ResetConsoleAttributes();
			if (PrintTimeLabel)
			{
				printf("]");
			}
			else
			{
				printf("] ");
			}
		}
		if (PrintTimeLabel)
		{
			printf("[");
			printf(GetTimeString(false).c_str());
			printf("] ");
		}
#if ColorLogMessages
		SetConsoleTextAttribute(ConsoleHandle, ConsoleSuccessTextAttributes);
#endif
		printf(Message);
		if (NewLine)
		{
			printf("\n");
		}
	}
	else
	{
		ConsoleLog("Invalid log type provided to ConsoleLog().", LOGERROR, false, true, true);
	}
	SetConsoleTextAttribute(ConsoleHandle, ConsoleDefaultTextAttributes);
}

static std::string GetEXEPath(bool IncludeExeInPath)
{
	char Result[MAX_PATH];
	GetModuleFileName(NULL, Result, MAX_PATH);
	std::string PathString(Result);

	if (IncludeExeInPath)
	{
		return PathString;
	}
	else
	{
		unsigned int ExeNameStart = PathString.length();
		for (unsigned int i = PathString.length(); i > 0; i--)
		{
			if (PathString.compare(i, 1, "\\") == 0 || PathString.compare(i, 1, "/") == 0)
			{
				ExeNameStart = i;
				break;
			}
		}
		PathString = PathString.substr(0, ExeNameStart + 1);
		return PathString;
	}
}

static void LoadDataFromConfig()
{
	std::string ExePath = GetEXEPath(false);
	if (fs::exists(ExePath + "RFGR Extended Camera/Settings.txt"))
	{
		// read a JSON file
		std::ifstream Config(ExePath + "RFGR Extended Camera/Settings.txt");
		Config >> MainConfig;
		Config.close();
	}
	else
	{
		CreateDirectoryIfNull(ExePath + "RFGR Extended Camera/");
		std::cout << "Settings.txt not found. Creating from default values." << std::endl;

		MainConfig["Default free camera speed"] = 0.3;
		MainConfig["Use debug console"] = false;
		//MainConfig["Default rotation speed"] = 0.7;
		//MainConfig["Create output console"] = true;

		//Keybinds
		/*MainConfig["Keybinds"]["Decrease camera speed"] = "q";
		MainConfig["Keybinds"]["Increase camera speed"] = "e";
		MainConfig["Keybinds"]["Toggle free camera"] = "VK_NUMPAD1";
		MainConfig["Keybinds"]["Toggle hud"] = "VK_NUMPAD2";
		MainConfig["Keybinds"]["Toggle fog"] = "VK_NUMPAD3";
		MainConfig["Keybinds"]["Camera forward"] = "VK_UP";
		MainConfig["Keybinds"]["Camera backward"] = "VK_DOWN";
		MainConfig["Keybinds"]["Camera left"] = "VK_LEFT";
		MainConfig["Keybinds"]["Camera right"] = "VK_RIGHT";
		MainConfig["Keybinds"]["Camera up"] = "z";
		MainConfig["Keybinds"]["Camera down"] = "x";

		//Rotation Keybinds		   
		MainConfig["Keybinds"]["Decrease rotation speed"] = "t";
		MainConfig["Keybinds"]["Increase rotation speed"] = "y";
		MainConfig["Keybinds"]["Toggle manual rotation control"] = "VK_NUMPAD4";
		MainConfig["Keybinds"]["Reset rotation"] = "VK_NUMPAD5";
		MainConfig["Keybinds"]["Lock pitch"] = "VK_NUMPAD7";
		MainConfig["Keybinds"]["Lock yaw"] = "VK_NUMPAD8";
		MainConfig["Keybinds"]["Lock roll"] = "VK_NUMPAD9";
		MainConfig["Keybinds"]["Decrease pitch"] = "VK_INSERT";
		MainConfig["Keybinds"]["Increase pitch"] = "VK_DELETE";
		MainConfig["Keybinds"]["Decrease yaw"] = "VK_END";
		MainConfig["Keybinds"]["Increase yaw"] = "VK_HOME";
		MainConfig["Keybinds"]["Decrease roll"] = "VK_NEXT";
		MainConfig["Keybinds"]["Increase roll"] = "VK_PRIOR";*/

		std::ofstream ConfigOutput(ExePath + "RFGR Extended Camera/Settings.txt");
		ConfigOutput << std::setw(4) << MainConfig << std::endl;
		ConfigOutput.close();
	}

	HKL KeyboardLayout = GetKeyboardLayout(0);
	//VkKeyScanEx(YOUR_CHAR, currentKBL);

	//ConsoleLog("Keyboard layout created", LOGWARNING, false, true, true);

	DefaultFreeCameraSpeed = MainConfig["Default free camera speed"].get<float>();
	UseDebugConsole = MainConfig["Use debug console"].get<bool>();
	//DefaultRotationSpeed = MainConfig["Default rotation speed"].get<float>();
	//UseConsole = MainConfig["Create output console"].get<bool>();

	/*ConsoleLog("Loading keybinds", LOGWARNING, false, true, true);

	KeyDecreaseCameraSpeed = VkKeyScanEx(MainConfig["Keybinds"]["Decrease camera speed"].get<int>(), KeyboardLayout);
	ConsoleLog("KeyDecreaseCameraSpeed Set", LOGERROR, false, true, true);
	KeyIncreaseCameraSpeed = VkKeyScanEx(MainConfig["Keybinds"]["Increase camera speed"].get<char>(), KeyboardLayout);
	KeyToggleFreecamera = VkKeyScanEx(MainConfig["Keybinds"]["Toggle free camera"].get<char>(), KeyboardLayout);
	KeyToggleHud = VkKeyScanEx(MainConfig["Keybinds"]["Toggle hud"].get<char>(), KeyboardLayout);
	KeyToggleFog = VkKeyScanEx(MainConfig["Keybinds"]["Toggle fog"].get<char>(), KeyboardLayout);
	KeyCameraForward = VkKeyScanEx(MainConfig["Keybinds"]["Camera forward"].get<char>(), KeyboardLayout);
	KeyCameraBackward = VkKeyScanEx(MainConfig["Keybinds"]["Camera backward"].get<char>(), KeyboardLayout);
	KeyCameraLeft = VkKeyScanEx(MainConfig["Keybinds"]["Camera left"].get<char>(), KeyboardLayout);
	KeyCameraRight = VkKeyScanEx(MainConfig["Keybinds"]["Camera right"].get<char>(), KeyboardLayout);
	KeyCameraUp = VkKeyScanEx(MainConfig["Keybinds"]["Camera up"].get<char>(), KeyboardLayout);
	KeyCameraDown = VkKeyScanEx(MainConfig["Keybinds"]["Camera down"].get<char>(), KeyboardLayout);

	ConsoleLog("Loading rotation control keybinds", LOGWARNING, false, true, true);

	KeyDecreaseRotationSpeed = VkKeyScanEx(MainConfig["Keybinds"]["Decrease rotation speed"].get<char>(), KeyboardLayout);
	KeyIncreaseRotationSpeed = VkKeyScanEx(MainConfig["Keybinds"]["Inrease rotation speed"].get<char>(), KeyboardLayout);
	KeyToggleRotationControl = VkKeyScanEx(MainConfig["Keybinds"]["Toggle manual rotation control"].get<char>(), KeyboardLayout);
	KeyResetRotation = VkKeyScanEx(MainConfig["Keybinds"]["Reset rotation"].get<char>(), KeyboardLayout);
	KeyLockPitch = VkKeyScanEx(MainConfig["Keybinds"]["Lock pitch"].get<char>(), KeyboardLayout);
	KeyLockYaw = VkKeyScanEx(MainConfig["Keybinds"]["Lock yaw"].get<char>(), KeyboardLayout);
	KeyLockRoll = VkKeyScanEx(MainConfig["Keybinds"]["Lock roll"].get<char>(), KeyboardLayout);
	KeyDecreasePitch = VkKeyScanEx(MainConfig["Keybinds"]["Decrease pitch"].get<char>(), KeyboardLayout);
	KeyIncreasePitch = VkKeyScanEx(MainConfig["Keybinds"]["Increase pitch"].get<char>(), KeyboardLayout);
	KeyDecreaseYaw = VkKeyScanEx(MainConfig["Keybinds"]["Decrease yaw"].get<char>(), KeyboardLayout);
	KeyIncreaseYaw = VkKeyScanEx(MainConfig["Keybinds"]["Increase yaw"].get<char>(), KeyboardLayout);
	KeyDecreaseRoll = VkKeyScanEx(MainConfig["Keybinds"]["Decrease roll"].get<char>(), KeyboardLayout);
	KeyIncreaseRoll = VkKeyScanEx(MainConfig["Keybinds"]["Increase roll"].get<char>(), KeyboardLayout);*/
}

static void PlaceNOP(BYTE* Address, DWORD Length = 1)
{
	DWORD OriginalProtectionPermissions;
	DWORD Backup;

	VirtualProtect(Address, Length, PAGE_EXECUTE_READWRITE, &OriginalProtectionPermissions);
	for (DWORD i = 0x0; i < Length; i++)
	{
		*(Address + i) = 0x90; //NOP is 0x90 in hex and 144 in decimal.
	}

	VirtualProtect(Address, Length, OriginalProtectionPermissions, &Backup); //Todo: Try using NULL or nullptr instead of Backup, since it's not needed.
}

static MODULEINFO GetModuleInfo(char* ModuleName)
{
	MODULEINFO ModuleInfo = { 0 };
	HMODULE ModuleHandle = GetModuleHandle(ModuleName);

	if (ModuleHandle == 0)
	{
		return ModuleInfo;
	}
	GetModuleInformation(GetCurrentProcess(), ModuleHandle, &ModuleInfo, sizeof(MODULEINFO));

	return ModuleInfo;
}

static DWORD FindPattern(char* ModuleName, char* Pattern, char* Mask)
{
	MODULEINFO ModuleInfo = GetModuleInfo(ModuleName);

	//Assign our base and module size
	//Having the values right is ESSENTIAL, this makes sure
	//that we don't scan unwanted memory and leading our game to crash
	DWORD BaseAddress = (DWORD)ModuleInfo.lpBaseOfDll;
	DWORD ModuleSize = (DWORD)ModuleInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	DWORD PatternLength = (DWORD)strlen(Mask);

	for (DWORD i = 0; i < ModuleSize - PatternLength; i++)
	{
		bool PatternFound = true;
		for (DWORD j = 0; j < PatternLength; j++)
		{
			//if we have a ? in our mask then we have true by default, 
			//or if the bytes match then we keep searching until finding it or not
			PatternFound &= Mask[j] == '?' || Pattern[j] == *(char*)(BaseAddress + i + j);
		}

		//found = true, our entire pattern was found
		//return the memory addy so we can write to it
		if (PatternFound)
		{
			return BaseAddress + i;
		}
	}
	ConsoleLog("FindPattern() returning NULL", LOGERROR, false, true);
	return NULL;
}