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

#define MY_LOG(format, ...) \
char log_format_dest[256];\
StringCchPrintfA(log_format_dest, 256, "[ AntRan GUI ] " __FUNCTION__ ": " format "\r\n", ##__VA_ARGS__);\
OutputDebugStringA(log_format_dest);

#endif //PCH_H
