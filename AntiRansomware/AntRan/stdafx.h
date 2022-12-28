#pragma once

#define EXTERN_C extern "C"
#define TIME_SECONDS(x) x * 10000000 * -1
#define MY_LOG(format, ...) DbgPrint("[ AntRan ] " __FUNCTION__ ": " format "\r\n", ##__VA_ARGS__);

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>