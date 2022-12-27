#include "stdafx.h"

NTSTATUS
MiniFltUnload(FLT_FILTER_UNLOAD_FLAGS flags);
FLT_PREOP_CALLBACK_STATUS
PrevCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID* completion_context);
FLT_POSTOP_CALLBACK_STATUS
PostCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID completion_context, FLT_POST_OPERATION_FLAGS flags);

PFLT_FILTER flt_handle;

FLT_OPERATION_REGISTRATION operations[] =
{
	{ IRP_MJ_CREATE, 0, PrevCreate, PostCreate, nullptr },
	{ IRP_MJ_OPERATION_END },
};

FLT_REGISTRATION registration =
{
	sizeof(registration),
	FLT_REGISTRATION_VERSION,
	0,
	nullptr,
	operations,
	MiniFltUnload,
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

NTSTATUS
MiniFltUnload(FLT_FILTER_UNLOAD_FLAGS flags)
{
	if (flt_handle)
	{
		FltUnregisterFilter(flt_handle);
	}

	KdPrint(("[ mini filter ]\t unload successful\r\n"));

	return STATUS_SUCCESS;
}

FLT_PREOP_CALLBACK_STATUS
PrevCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID* completion_context)
{
	NTSTATUS status = STATUS_SUCCESS;
	PFLT_FILE_NAME_INFORMATION file_name_info = nullptr;

	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &file_name_info);
	if (!NT_SUCCESS(status))
	{
		return FLT_PREOP_SUCCESS_NO_CALLBACK;
	}

	status = FltParseFileNameInformation(file_name_info);
	if (!NT_SUCCESS(status))
	{
		FltReleaseFileNameInformation(file_name_info);
		return FLT_PREOP_SUCCESS_NO_CALLBACK;
	}

	if (file_name_info->Name.MaximumLength >= 260)
	{
		FltReleaseFileNameInformation(file_name_info);
		return FLT_PREOP_SUCCESS_NO_CALLBACK;
	}

	WCHAR name[260] = { 0, };
	RtlCopyMemory(name, file_name_info->Name.Buffer, file_name_info->Name.MaximumLength);
	_wcsupr(name);
	if (wcsstr(name, L".TXT") == nullptr)
	{
		FltReleaseFileNameInformation(file_name_info);
		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
	}

	data->IoStatus.Status = STATUS_ACCESS_DENIED;
	FltReleaseFileNameInformation(file_name_info);
	return FLT_PREOP_COMPLETE;
}

FLT_POSTOP_CALLBACK_STATUS
PostCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS flt_obj, PVOID completion_context, FLT_POST_OPERATION_FLAGS flags)
{
	//KdPrint(("[ mini filter ]\t " __FUNCTION__ "\t%u -> (%wZ)\r\n"), PtrToUint(PsGetCurrentProcessId()), &file_name_info->FinalComponent);

	return FLT_POSTOP_FINISHED_PROCESSING;
}

VOID
DriverUnload(PDRIVER_OBJECT driver_obj)
{
	KdPrint(("[ driver ]\t unload successful\r\n"));
}

EXTERN_C NTSTATUS
DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registry_path)
{
	NTSTATUS status = STATUS_SUCCESS;

	driver_obj->DriverUnload = DriverUnload;

	status = FltRegisterFilter(driver_obj, &registration, &flt_handle);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	status = FltStartFiltering(flt_handle);
	if (!NT_SUCCESS(status))
	{
		FltUnregisterFilter(flt_handle);
		return status;
	}

	KdPrint(("[ driver ]\t load successful\r\n"));
	KdPrint(("[ mini filter ]\t load successful\r\n"));

	return status;
}