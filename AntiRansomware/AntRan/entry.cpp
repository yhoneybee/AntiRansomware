#include "stdafx.h"
#include "filter.h"

VOID
DriverUnload(PDRIVER_OBJECT driver_obj)
{
	MY_LOG("unload successful.");
}

EXTERN_C NTSTATUS
DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registry_path)
{
	NTSTATUS status = STATUS_SUCCESS;

	driver_obj->DriverUnload = DriverUnload;

	status = FilterLoad(driver_obj);
	if (!NT_SUCCESS(status))
	{
		MY_LOG("load failed.");
		return status;
	}

	MY_LOG("load successful.");

	return status;
}