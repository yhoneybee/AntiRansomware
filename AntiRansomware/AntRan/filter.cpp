#include "stdafx.h"
#include "filter.h"
#include <SharedData.h>

PFLT_FILTER filter;

PFLT_PORT filter_port;
PFLT_PORT client_port;

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
		MY_LOG("FltRegisterFilter function was failed.");
		return status;
	}

	status = FilterPortOpen();
	if (!NT_SUCCESS(status))
	{
		MY_LOG("FilterPortOpen function was failed.");
		FltUnregisterFilter(filter);
		return status;
	}

	status = FltStartFiltering(filter);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("FltStartFiltering function was failed.");
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
	RtlZeroMemory(&port_name, sizeof(port_name));
	RtlInitUnicodeString(&port_name, PORT_NAME);

	PSECURITY_DESCRIPTOR security_descriptor;
	RtlZeroMemory(&security_descriptor, sizeof(security_descriptor));
	status = FltBuildDefaultSecurityDescriptor(&security_descriptor, FLT_PORT_ALL_ACCESS);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("FltBuildDefaultSecurityDescriptor function was failed.");
		FilterPortClose();
		return status;
	}

	OBJECT_ATTRIBUTES obj_attributes;
	RtlZeroMemory(&obj_attributes, sizeof(obj_attributes));
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

NTSTATUS PortSend(PVOID data, ULONG data_size, PVOID recv_buf, ULONG recv_buf_size, PULONG written_recv_buf)
{
	NTSTATUS status = STATUS_SUCCESS;

	if (recv_buf)
	{
		status = FltSendMessage(filter, &client_port, data, data_size, recv_buf, &recv_buf_size, nullptr);
		if (!NT_SUCCESS(status))
		{
			MY_LOG("FltSendMessage(recv_buf was not null) function was failed(%dl).", status);
			return status;
		}

		*written_recv_buf = recv_buf_size;
	}
	else
	{
		LARGE_INTEGER timeout;
		timeout.QuadPart = 0;

		status = FltSendMessage(filter, &client_port, data, data_size, nullptr, nullptr, &timeout);
		if (!NT_SUCCESS(status))
		{
			MY_LOG("FltSendMessage(recv_buf was null) function was failed(%dl).", status);
			return status;
		}

		*written_recv_buf = 0;
	}

	MY_LOG("PortSend function was successful.");

	return status;
}

FLT_PREOP_CALLBACK_STATUS PrevCreateRoutine(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID* context)
{
	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS PostCreateRoutine(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID context, FLT_POST_OPERATION_FLAGS flags)
{
	if (flt_obj == nullptr || flt_obj->FileObject == nullptr || flt_obj->FileObject->FileName.Buffer == nullptr)
	{
		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	PFLT_FILE_NAME_INFORMATION file_name_info = nullptr;
	NTSTATUS status;

	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &file_name_info);
	if (!NT_SUCCESS(status))
	{
		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	status = FltParseFileNameInformation(file_name_info);
	if (!NT_SUCCESS(status))
	{
		FltReleaseFileNameInformation(file_name_info);
		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	Filter2User sent;
	RtlZeroMemory(&sent, sizeof(sent));

	User2Filter reply;
	RtlZeroMemory(&reply, sizeof(reply));

	ULONG returned_bytes = 0;

	wcscpy_s(sent.path, ARRAYSIZE(sent.path), file_name_info->Name.Buffer);

	status = PortSend(&sent, sizeof(sent), &reply, sizeof(reply), &returned_bytes);
	if (NT_SUCCESS(status) && returned_bytes > 0 && reply.block)
	{
		data->IoStatus.Status = STATUS_ACCESS_DENIED;
		FltReleaseFileNameInformation(file_name_info);
		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	FltReleaseFileNameInformation(file_name_info);
	return FLT_POSTOP_FINISHED_PROCESSING;
}

NTSTATUS PortNotifyConnectRoutine(PFLT_PORT client, PVOID cookie, PVOID context, ULONG context_size, PVOID* port_cookie)
{
	MY_LOG("user mode application(%u) was connected.", PtrToUint(PsGetCurrentProcessId()));

	client_port = client;

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
