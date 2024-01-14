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

bool IsKeyPressed(int vkCode)
{
	return keyStates[vkCode];
}

void SetupHook() {
		//MSGBOX("SetupHook");
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
}