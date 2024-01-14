#pragma once
#include <map>

extern std::map<int, bool> keyStates;

bool IsKeyPressed(int vkCode);

void SetupHook();