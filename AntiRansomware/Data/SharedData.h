#pragma once
#include <tchar.h>

const WCHAR* PORT_NAME_W = L"AntRanPORT";
const CHAR* PORT_NAME_A = "AntRanPORT";

constexpr int BUF_SIZE = 4096 / sizeof(WCHAR);

typedef struct
{
	WCHAR data[BUF_SIZE];
	INT length;
} Message, *PMessage;