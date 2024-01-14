#pragma once
#include <map>
#include "Structs.h"
#include <Windows.h>

extern std::map<int, bool> keyStates;

struct WindowSize {
	int width;
	int height;
};


bool IsKeyPressed(int vkCode);

void SetupHook();

bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight);

void Simulate_LeftClick();

void SimulateSpacebarPress();

WindowSize GetWinSize(DWORD ProcessId);