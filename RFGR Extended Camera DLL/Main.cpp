//#include "Globals.hpp"
//#include "SnippetManager.hpp"
#include "CameraWrapper.hpp"

//std::ofstream LogFile;
static bool PreExistingConsole = true;

DWORD WINAPI MainThread(LPVOID Param)
{
	if (UseDebugConsole)
	{
		FILE *pFile = nullptr;
		DWORD PID = GetCurrentProcessId();

		if (AttachConsole(PID) == 0)
		{
			PreExistingConsole = false;
			AllocConsole();
			
		}
		else
		{
			PreExistingConsole = true;
		}
		freopen_s(&pFile, "CONOUT$", "r+", stdout);
	}

	ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleAttributes(ConsoleDefaultTextAttributes);

	LoadDataFromConfig();

	ConsoleLog("RFGR extended camera loaded.\n", LOGSUCCESS, true, true);

    uintptr_t ModuleBase = (uintptr_t)GetModuleHandle(NULL);
	//std::cout << "ModuleBase, hex: " << std::hex << std::uppercase << ModuleBase << std::dec << ", decimal: " << ModuleBase << std::endl;

    HideHud = (F_HideHud)(ModuleBase + 0x441A90);
	ShowFog = (F_ShowFog)(ModuleBase + 0x3C2C70);

	CameraWrapper Camera;
	Camera.Initialize(DefaultFreeCameraSpeed, 5.0);

	unsigned int ExitKeysPressCount = 0;

	bool HudVisible = true;
	bool FogVisible = true;

	ConsoleLog("Initialization Complete\n", LOGSUCCESS, false, true);
	int MovementSleepTime = 4;

	while (ExitKeysPressCount < 5)
	{
		if (Camera.IsFreeCameraActive())
		{
			if (GetAsyncKeyState(0x51) & 0x8000) //q
			{
				Camera.AdjustCameraSpeed(-0.02f);
				Sleep(50);
			}
			if (GetAsyncKeyState(0x45) & 0x8000) //e
			{
				Camera.AdjustCameraSpeed(0.02f);
				Sleep(50);
			}
			if (GetAsyncKeyState(0x54) & 0x8000) //t
			{
				Camera.AdjustCameraRotationSpeed(-0.02f);
				Sleep(50);
			}
			if (GetAsyncKeyState(0x59) & 0x8000) //y
			{
				Camera.AdjustCameraRotationSpeed(0.02f);
				Sleep(50);
			}
			if (GetKeyState(0x5A) & 0x8000) //z
			{
				Camera.MoveFreeCamera(DOWN);
				Sleep(MovementSleepTime);
			}
			if (GetAsyncKeyState(0x58) & 0x8000) //x
			{
				Camera.MoveFreeCamera(UP);
				Sleep(MovementSleepTime);
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000) //Up arrow key
			{
				Camera.MoveFreeCamera(FORWARD);
				Sleep(MovementSleepTime);
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) //Down arrow key
			{
				Camera.MoveFreeCamera(BACKWARD);
				Sleep(MovementSleepTime);
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) //Right arrow key
			{
				Camera.MoveFreeCamera(RIGHT);
				Sleep(MovementSleepTime);
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) //Left arrow key
			{
				Camera.MoveFreeCamera(LEFT);
				Sleep(MovementSleepTime);
			}
		}

		if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000))
		{
			ExitKeysPressCount++;
			Sleep(100);
		}
		if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		{
			Camera.ToggleFreeCamera();
			Sleep(150);
		}
		if (GetKeyState(VK_NUMPAD2) & 0x8000)
		{ 
			ConsoleLog("Toggling HUD", LOGMESSAGE, false, true, true);
			HideHud(HudVisible);
			HudVisible = !HudVisible;
			Sleep(150);
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
        {
			ConsoleLog("Toggling Fog", LOGMESSAGE, false, true, true);
			FogVisible = !FogVisible;
			ShowFog(FogVisible);
			Sleep(150);
        }
    }
	
	ConsoleLog("RFGR extended camera unloaded.\n\n", LOGSUCCESS, true, true);

	//Clean up everything, including restoring an NOP'd pieces of code.
	if (PreExistingConsole)
	{
		FreeConsole();
	}
	if (Camera.IsFreeCameraActive())
	{
		Camera.DeactivateFreeCamera();
		Camera.DeactivateRotationControl();
	}
	if (!HudVisible)
	{
		HideHud(false);
		ShowFog(true);
	}

    FreeLibraryAndExitThread((HMODULE)Param, 0);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, MainThread, hModule, 0, 0);
            break;
        default:
            break;
    }
    return TRUE;
}