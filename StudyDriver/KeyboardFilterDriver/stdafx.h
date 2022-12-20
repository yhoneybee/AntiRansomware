#pragma once

#define EXTERN_C extern "C"

#include <ntddk.h>
#include <tchar.h>
#include <strsafe.h>

typedef struct
{
	PDEVICE_OBJECT lower_keyboard_device;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;