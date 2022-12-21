#pragma once

#define EXTERN_C extern "C"
#define TIME_SECONDS(x) x * 10000000 * -1

#include <ntddk.h>
#include <tchar.h>
#include <strsafe.h>

typedef struct
{
	PDEVICE_OBJECT lower_keyboard_device;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

typedef struct
{
	USHORT UnitId;
	USHORT MakeCode;
	USHORT Flags;
	USHORT Reserved;
	ULONG ExtraInformation;
} KEYBOARD_INPUT_DATA, *PKEYBOARD_INPUT_DATA;