#include "Utils.h"
#include <Windows.h>
#include <map>


HHOOK hHook = NULL;
std::map<int, bool> keyStates;
extern DWORD ProcessId;

HWND FindWindowByProcessId(DWORD processId)
{
	HWND hwnd = NULL;

	EnumWindows([](HWND handle, LPARAM lParam) -> BOOL {
		DWORD processId;
		GetWindowThreadProcessId(handle, &processId);

		// If the process ID matches, set the HWND and return FALSE to stop enumeration
		if (processId == lParam)
		{
			*((HWND*)lParam) = handle;
			return FALSE;
		}

		return TRUE;  // Continue enumeration
		}, (LPARAM)&processId);

	return hwnd;
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

bool IsKeyPressed(int vkCode)
{
	return keyStates[vkCode];
}