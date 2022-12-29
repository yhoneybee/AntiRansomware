#pragma once
#include "pch.h"

constexpr auto PORT_NAME = L"\\AntRanPORT";

constexpr int BUF_SIZE = 4096 / sizeof(wchar_t);

typedef struct NAME_EXPORT
{
	wchar_t data[BUF_SIZE];
} Message, *PMessage;

typedef struct NAME_EXPORT
{
	wchar_t path[260];
} Filter2User, *PFilter2User;

typedef struct NAME_EXPORT
{
	bool block;
} User2Filter, *PUser2Filter;