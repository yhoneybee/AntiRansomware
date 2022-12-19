#include "stdafx.h"
#include <ntddk.h>

#define DEVICE_SEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_DATA)

UNICODE_STRING device_name = RTL_CONSTANT_STRING(L"\\Device\\mydevice");
UNICODE_STRING symbol_name = RTL_CONSTANT_STRING(L"\\??\\mysymbol");

PDEVICE_OBJECT device_obj = nullptr;

EXTERN_C VOID Unload(PDRIVER_OBJECT driver_obj)
{
	IoDeleteSymbolicLink(&symbol_name);
	IoDeleteDevice(device_obj);
	KdPrint(("driver unload successful.\r\n"));
}

EXTERN_C NTSTATUS DispatchPassThrough(PDEVICE_OBJECT device_obj, PIRP irp)
{
	PIO_STACK_LOCATION io_stack_location = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS status = STATUS_SUCCESS;

	switch (io_stack_location->MajorFunction)
	{
	case IRP_MJ_CREATE:
		KdPrint(("create request.\r\n"));
		break;
	case IRP_MJ_CLOSE:
		KdPrint(("close request.\r\n"));
		break;
	case IRP_MJ_READ:
		KdPrint(("read request.\r\n"));
		break;
	default:
		status = STATUS_INVALID_PARAMETER;
		break;
	}

	irp->IoStatus.Information = 0;
	irp->IoStatus.Status = status;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status;
}

EXTERN_C NTSTATUS DispatchDeviceControl(PDEVICE_OBJECT device_obj, PIRP irp)
{
	PIO_STACK_LOCATION io_stack_location = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS status = STATUS_SUCCESS;

	PVOID buffer = irp->AssociatedIrp.SystemBuffer;
	size_t length = 0;
	ULONG in_length = io_stack_location->Parameters.DeviceIoControl.InputBufferLength;
	ULONG out_length = io_stack_location->Parameters.DeviceIoControl.OutputBufferLength;
	LPWSTR temp = L"sample returned from driver";

	switch (io_stack_location->Parameters.DeviceIoControl.IoControlCode)
	{
	case DEVICE_SEND:
		KdPrint(("send data is %ws\r\n", (LPWSTR)buffer));
		StringCchLengthW((LPWSTR)buffer, 1024, &length);
		break;
	case DEVICE_READ:
		StringCbCopyNW((LPWSTR)buffer, 1024 * sizeof(WCHAR), temp, 28 * sizeof(WCHAR));
		StringCbLengthW((LPWSTR)buffer, 1024 * sizeof(WCHAR), &length);
		break;
	default:
		status = STATUS_INVALID_PARAMETER;
		break;
	}

	irp->IoStatus.Status = status;
	irp->IoStatus.Information = length;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status;
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registry_path)
{
	NTSTATUS status;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		driver_obj->MajorFunction[i] = DispatchPassThrough;
	}

	driver_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;

	driver_obj->DriverUnload = Unload;

	status = IoCreateDevice(driver_obj, 0, &device_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, false, &device_obj);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("creating device was failed...\r\n"));
		return status;
	}

	status = IoCreateSymbolicLink(&symbol_name, &device_name);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("creating symbol was failed...\r\n"));
		IoDeleteDevice(device_obj);
		return status;
	}

	//driver_obj->MajorFunction[IRP_MJ_WRITE] = DispatchCustom;

	KdPrint(("driver load successful.\r\n"));

	return status;
}