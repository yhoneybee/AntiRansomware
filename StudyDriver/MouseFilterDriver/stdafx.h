#pragma once

#define EXTERN_C extern "C"
#define TIME_SECONDS(x) x * 10000000 * -1

#include <ntddk.h>
#include <ntddmou.h>
#include <tchar.h>
#include <strsafe.h>

typedef struct
{
	PDEVICE_OBJECT lower_device;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;