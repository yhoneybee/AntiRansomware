#pragma once

#ifdef EXPORTABLE_DATA
#define NAME_EXPORT __declspec(dllexport)
#else
#define NAME_EXPORT __declspec(dllimport)
#endif

#include <tchar.h>