#include "Utils.h"
#include <sstream>
#include <Windows.h>
#include <map>

#define MSGBOX(x) \
{ \
   std::ostringstream oss; \
   oss << x; \
   std::string str = oss.str(); \
   std::wstring stemp = std::wstring(str.begin(), str.end()); \
   LPCWSTR sw = stemp.c_str(); \
   MessageBox(NULL, sw, L"Msg Title", MB_OK | MB_ICONQUESTION); \
}


HHOOK hHook = NULL;
std::map<int, bool> keyStates;
extern DWORD ProcessId;

struct Params
{
	DWORD pid;
	HWND hwnd;
};

HWND FindWindowByProcessId(DWORD processId)
{
	Params params = { processId, NULL };

	EnumWindows([](HWND handle, LPARAM lParam) -> BOOL {
		DWORD processId;
		GetWindowThreadProcessId(handle, &processId);
		Params* params = (Params*)lParam;

		// If the process ID matches, set the HWND and return FALSE to stop enumeration
		if (processId == params->pid)
		{
			params->hwnd = handle;  // Set hwnd
			return FALSE;
		}

		return TRUE;  // Continue enumeration
		}, (LPARAM)&params);

	return params.hwnd;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		HWND hCurrentWindow = GetForegroundWindow();
		HWND hGameWindow = FindWindowByProcessId(ProcessId);

		if (hCurrentWindow == hGameWindow)
		{
			KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
			if (wParam == WM_KEYDOWN)
			{
				keyStates[p->vkCode] = true;
			}
			else if (wParam == WM_KEYUP)
			{
				keyStates[p->vkCode] = false;
			}
		}
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void SimulateSpacebarPress()
{
	INPUT input[2] = { 0 };

	// Set up the INPUT structures for a keydown and keyup event
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_SPACE;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_SPACE;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;

	// Send the input

	HWND hCurrentWindow = GetForegroundWindow();
	HWND hGameWindow = FindWindowByProcessId(ProcessId);

	// Check if the game is in focus
	if (hCurrentWindow == hGameWindow)
	{
		SendInput(2, input, sizeof(INPUT));
	}
}

void Simulate_LeftClick()
{
	INPUT Input = { 0 };

	// Set up a generic mouse event.
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	//SendInput(1, &Input, sizeof(INPUT));

	// Zero the memory
	ZeroMemory(&Input, sizeof(INPUT));

	// Set up a generic mouse event.
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	HWND hCurrentWindow = GetForegroundWindow();
	HWND hGameWindow = FindWindowByProcessId(ProcessId);

	// Check if the game is in focus <--- To prevent the mouse from clicking on other windows
	if (hCurrentWindow == hGameWindow)
	{
		SendInput(1, &Input, sizeof(INPUT));
	}
}


WindowSize GetWinSize(DWORD ProcessId)
{
	WindowSize WinSize = { 0, 0 };

	HWND hGameWindow = FindWindowByProcessId(ProcessId);

	RECT rect;
	if (GetWindowRect(hGameWindow, &rect))
	{
		WinSize.width = rect.right - rect.left;
		WinSize.height = rect.bottom - rect.top;
	}

	return WinSize;
}

bool IsKeyPressed(int vkCode)
{
	return keyStates[vkCode];
}

void SetupHook() {
		//MSGBOX("SetupHook");
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
}

bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vec4 clipCoords = {};

	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
	{
		return false;
	}

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}