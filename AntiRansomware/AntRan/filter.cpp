#include "stdafx.h"
#include "filter.h"
#include <SharedData.h>

PFLT_FILTER filter;

PFLT_PORT filter_port;

FLT_OPERATION_REGISTRATION operations[] =
{
	{ IRP_MJ_CREATE, 0, PrevCreateRoutine, PostCreateRoutine },
	{ IRP_MJ_OPERATION_END },
};

FLT_REGISTRATION registration =
{
	sizeof(registration),
	FLT_REGISTRATION_VERSION,
	0,
	nullptr,
	operations,
	FilterUnload,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr
};

NTSTATUS FilterLoad(PDRIVER_OBJECT driver_obj)
{
	NTSTATUS status = STATUS_SUCCESS;

	status = FltRegisterFilter(driver_obj, &registration, &filter);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("register filter was failed.");
		return status;
	}

	status = FltStartFiltering(filter);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("start filtering was failed.");
		FltUnregisterFilter(filter);
		return status;
	}

	MY_LOG("load successful.");

	return status;
}

NTSTATUS FilterUnload(FLT_FILTER_UNLOAD_FLAGS flags)
{
	FilterPortClose();

	if (filter)
	{
		FltUnregisterFilter(filter);
	}

	MY_LOG("unload successful.");

	return STATUS_SUCCESS;
}

NTSTATUS FilterPortOpen()
{
	NTSTATUS status = STATUS_SUCCESS;

	UNICODE_STRING port_name;
	RtlZeroMemory(&port_name, sizeof(UNICODE_STRING));
	RtlInitUnicodeString(&port_name, PORT_NAME_W);

	PSECURITY_DESCRIPTOR security_descriptor;
	RtlZeroMemory(&security_descriptor, sizeof(PSECURITY_DESCRIPTOR));
	status = FltBuildDefaultSecurityDescriptor(&security_descriptor, FLT_PORT_ALL_ACCESS);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("FltBuildDefaultSecurityDescriptor function was failed.");
		FilterPortClose();
		return status;
	}

	OBJECT_ATTRIBUTES obj_attributes;
	RtlZeroMemory(&obj_attributes, sizeof(OBJECT_ATTRIBUTES));
	InitializeObjectAttributes(&obj_attributes, &port_name, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, nullptr, security_descriptor);

	status = FltCreateCommunicationPort(filter, &filter_port, &obj_attributes, nullptr, PortNotifyConnectRoutine, PortNotifyDisconnectRoutine, PortNotifyMessageRoutine, 1);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("FltCreateCommunicationPort function was failed.");
		FilterPortClose();
		return status;
	}

	MY_LOG("port was opened.");

	return status;
}

VOID FilterPortClose()
{
	if (filter_port)
	{
		FltCloseCommunicationPort(filter_port);
	}

	MY_LOG("close successful.");
}

FLT_PREOP_CALLBACK_STATUS PrevCreateRoutine(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID* context)
{
	if (flt_obj && flt_obj->FileObject && flt_obj->FileObject->FileName.Buffer)
	{
		MY_LOG("%u -> (%wZ)", PtrToUint(PsGetCurrentProcessId()), (PUNICODE_STRING)&flt_obj->FileObject->FileName);
	}

	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS PostCreateRoutine(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID context, FLT_POST_OPERATION_FLAGS flags)
{
	if (flt_obj && flt_obj->FileObject && flt_obj->FileObject->FileName.Buffer)
	{
		MY_LOG("%u -> (%wZ)", PtrToUint(PsGetCurrentProcessId()), (PUNICODE_STRING)&flt_obj->FileObject->FileName);
	}

	return FLT_POSTOP_FINISHED_PROCESSING;
}

NTSTATUS PortNotifyConnectRoutine(PFLT_PORT client, PVOID cookie, PVOID context, ULONG context_size, PVOID* port_cookie)
{
	MY_LOG("user mode application(%u) was connected.", PtrToUint(PsGetCurrentProcessId()));

	return STATUS_SUCCESS;
}

VOID PortNotifyDisconnectRoutine(PVOID cookie)
{
	MY_LOG("user mode application(%u) was disconnected.", PtrToUint(PsGetCurrentProcessId()));
}

NTSTATUS PortNotifyMessageRoutine(PVOID port_cookie, PVOID input_buf, ULONG input_buf_size, PVOID output_buf, ULONG output_buf_size, PULONG return_output_buf_length)
{
	MY_LOG("user mode application(%u) was sent data.", PtrToUint(PsGetCurrentProcessId()));

	if (input_buf && input_buf_size == sizeof(Message))
	{
		PMessage sent = (PMessage)input_buf;
		MY_LOG("message: %ws", sent->data);
	}

	return STATUS_SUCCESS;
}
