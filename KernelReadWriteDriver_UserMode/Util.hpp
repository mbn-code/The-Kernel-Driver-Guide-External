#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

inline std::wstring to_wide(const char* ps) { return std::wstring(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>(ps).from_bytes(ps)); }