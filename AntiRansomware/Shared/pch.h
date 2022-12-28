#pragma once

#ifdef EXPORTABLE_SHARED
#define NAME_EXPORT __declspec(dllexport)
#else
#define NAME_EXPORT __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>