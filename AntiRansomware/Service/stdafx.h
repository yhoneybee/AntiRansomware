#pragma once

// macro
#define SAFE_DELETE(p) { if(p != nullptr) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p != nullptr) { delete[] p; p = nullptr; } }
#ifdef EXPORTABLE_SERVICE
#define NAME_EXPORT __declspec(dllexport)
#else
#define NAME_EXPORT __declspec(dllimport)
#endif

//	include
//		C++
#include <iostream>

//		C
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdarg.h>
#include <setupapi.h>
#pragma comment(lib, "Setupapi.lib")
#include <winsvc.h>

//	alias
//		typedef


//		using


//	constant value
constexpr int NULL_SPACE = 1;
constexpr int INT_MAXIMUM_POWER = 9;

//	global functions
NAME_EXPORT DWORD PrintErrorMessage(DWORD error);
NAME_EXPORT DWORD PrintErrorMessage(DWORD error, LPCTSTR format);