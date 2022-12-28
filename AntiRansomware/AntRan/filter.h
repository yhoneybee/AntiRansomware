#pragma once

NTSTATUS
FilterLoad(PDRIVER_OBJECT driver_obj);

NTSTATUS
FilterUnload(FLT_FILTER_UNLOAD_FLAGS flags);

NTSTATUS
FilterPortOpen();

VOID
FilterPortClose();

FLT_PREOP_CALLBACK_STATUS
PrevCreateRoutine(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID* context);

FLT_POSTOP_CALLBACK_STATUS
PostCreateRoutine(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID context, FLT_POST_OPERATION_FLAGS flags);

NTSTATUS
PortNotifyConnectRoutine(PFLT_PORT client, PVOID cookie, PVOID context, ULONG context_size, PVOID* port_cookie);

VOID
PortNotifyDisconnectRoutine(PVOID cookie);

NTSTATUS
PortNotifyMessageRoutine(PVOID port_cookie, PVOID input_buf, ULONG input_buf_size, PVOID output_buf, ULONG output_buf_size, PULONG return_output_buf_length);