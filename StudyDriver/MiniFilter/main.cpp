#include "stdafx.h"

PFLT_FILTER filter = nullptr;

NTSTATUS MFUnload(FLT_FILTER_UNLOAD_FLAGS flags);
FLT_PREOP_CALLBACK_STATUS MFPreCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS filter_objs, PVOID* completion_context);
FLT_POSTOP_CALLBACK_STATUS MFPostCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS filter_objs, PVOID completion_context, FLT_POST_OPERATION_FLAGS flags);
FLT_PREOP_CALLBACK_STATUS MFPreWrite(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS filter_objs, PVOID* completion_context);

const FLT_OPERATION_REGISTRATION callbacks[] =
{
	{IRP_MJ_CREATE, 0, MFPreCreate, MFPostCreate},
	{IRP_MJ_OPERATION_END}
};

const FLT_REGISTRATION filter_registration =
{
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	0,
	nullptr,
	callbacks,
	MFUnload,
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

NTSTATUS MFUnload(FLT_FILTER_UNLOAD_FLAGS flags)
{
	FltUnregisterFilter(filter);

	KdPrint(("[ MF ]\t minifilter unload successful\r\n"));

	return STATUS_SUCCESS;
}

FLT_PREOP_CALLBACK_STATUS MFPreCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS filter_objs, PVOID* completion_context)
{
	KdPrint(("[ MF ]\t MFPreCreate\r\n"));

	NTSTATUS status = STATUS_SUCCESS;

	WCHAR name[260] = { 0, };

	PFLT_FILE_NAME_INFORMATION file_name_info;
	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &file_name_info);
	if (NT_SUCCESS(status))
	{
		status = FltParseFileNameInformation(file_name_info);
		if (NT_SUCCESS(status))
		{
			if (file_name_info->Name.MaximumLength < 260)
			{
				RtlCopyMemory(name, file_name_info->Name.Buffer, file_name_info->Name.MaximumLength);
				KdPrint(("[ MF ]\t create file: %ws\r\n"), name);
			}
		}

		FltReleaseFileNameInformation(file_name_info);
	}

	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS MFPostCreate(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS filter_objs, PVOID completion_context, FLT_POST_OPERATION_FLAGS flags)
{
	KdPrint(("[ MF ]\t pos create is running\r\n"));

	return FLT_POSTOP_FINISHED_PROCESSING;
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registry_path)
{
	NTSTATUS status = STATUS_SUCCESS;

	status = FltRegisterFilter(driver_obj, &filter_registration, &filter);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	status = FltStartFiltering(filter);
	if (!NT_SUCCESS(status))
	{
		FltUnregisterFilter(filter);
		return status;
	}

	KdPrint(("[ MF ]\t minifilter load successful\r\n"));
}