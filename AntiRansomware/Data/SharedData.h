#pragma once
#include "pch.h"

const wchar_t* PORT_NAME_W = L"AntRanPORT";
const char* PORT_NAME_A = "AntRanPORT";

constexpr int BUF_SIZE = 4096 / sizeof(wchar_t);

typedef struct NAME_EXPORT
{
	wchar_t data[BUF_SIZE];
} Message, *PMessage;