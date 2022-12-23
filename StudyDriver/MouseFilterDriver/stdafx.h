#pragma once

#define EXTERN_C extern "C"
#define TIME_SECONDS(x) x * 10000000 * -1

#include <ntddk.h>
#include <tchar.h>
#include <strsafe.h>

typedef struct
{
	PDEVICE_OBJECT lower_device;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

typedef struct _MOUSE_INPUT_DATA {
    USHORT UnitId;
    USHORT Flags;
    union {
        ULONG Buttons;
        struct {
            USHORT ButtonFlags;
            USHORT ButtonData;
        };
    };
    ULONG  RawButtons;
    LONG   LastX;
    LONG   LastY;
    ULONG  ExtraInformation;
} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;