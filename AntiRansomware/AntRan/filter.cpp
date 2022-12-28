#include "stdafx.h"
#include "filter.h"

PFLT_FILTER filter;

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
	if (filter)
	{
		FltUnregisterFilter(filter);
	}

	MY_LOG("unload successful.");

	return STATUS_SUCCESS;
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
