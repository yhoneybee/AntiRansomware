#pragma once

// macro
#define SAFE_DELETE(p) { if(p != nullptr) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p != nullptr) { delete[] p; p = nullptr; } }

//	include
//		C++
#include <iostream>

//		C
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdarg.h>

//	alias
//		typedef


//		using


//	constant value
constexpr int NULL_SPACE = 1;
constexpr int INT_MAXIMUM_POWER = 9;

//	global functions
DWORD GetErrorMessage(DWORD error);
DWORD GetErrorMessage(DWORD error, LPCTSTR format);