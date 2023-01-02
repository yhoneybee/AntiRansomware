// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <fltUser.h>
#include <strsafe.h>
#include <SharedData.h>

#include <thread>

#define WM_TRAY_NOTIFYICACTION			WM_APP + 1

typedef struct
{
	FILTER_MESSAGE_HEADER hdr;
	Filter2User data;
} Filter2UserWrapper;

typedef struct
{
	FILTER_REPLY_HEADER hdr;
	User2Filter data;
} User2FilterWrapper;

#define GET_LOG_STRING(format, ...)\
TCHAR log_format_dest[256];\
StringCchPrintf(log_format_dest, 256, "[ AntRan GUI ] " __FUNCTION__ ": " L ## format "\r\n", ##__VA_ARGS__);\

#define MY_LOG(format, ...) \
GET_LOG_STRING(format, ##__VA_ARGS__)\
OutputDebugString(log_format_dest);

#define MY_MESSAGEBOX(format, ...)\
GET_LOG_STRING(format, ##__VA_ARGS__);\
MessageBox(nullptr, log_format_dest, L"", MB_OK);

#endif //PCH_H
