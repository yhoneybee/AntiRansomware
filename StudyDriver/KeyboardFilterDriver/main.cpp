#include "stdafx.h"

PDEVICE_OBJECT keyboard_device = nullptr;

VOID Unload(PDRIVER_OBJECT driver)
{
	PDEVICE_OBJECT device = driver->DeviceObject;
	IoDetachDevice(((PDEVICE_EXTENSION)device->DeviceExtension)->lower_keyboard_device);
	IoDetachDevice(keyboard_device);
	KdPrint(("[ KFD ]\t unload was successful\r\n"));
}

NTSTATUS AttachDevice(PDRIVER_OBJECT driver)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING target_device = RTL_CONSTANT_STRING(L"\\Device\\KeyboardClass0");

	status = IoCreateDevice(driver, sizeof(DEVICE_EXTENSION), nullptr, FILE_DEVICE_KEYBOARD, 0, false, &keyboard_device);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	keyboard_device->Flags |= DO_BUFFERED_IO;
	keyboard_device->Flags &= ~DO_DEVICE_INITIALIZING;

	RtlZeroMemory(keyboard_device->DeviceExtension, sizeof(DEVICE_EXTENSION));

	status = IoAttachDevice(keyboard_device, &target_device, &((PDEVICE_EXTENSION)(keyboard_device->DeviceExtension))->lower_keyboard_device);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(keyboard_device);
		return status;
	}

	return status;
}

NTSTATUS DispatchPassThrough(PDEVICE_OBJECT device, PIRP irp)
{
	NTSTATUS status = STATUS_SUCCESS;

	return status;
}

NTSTATUS DispatchRead(PDEVICE_OBJECT device, PIRP irp)
{
	NTSTATUS status = STATUS_SUCCESS;

	return status;
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING registry_path)
{
	NTSTATUS status = STATUS_SUCCESS;

	driver->DriverUnload = Unload;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		driver->MajorFunction[i] = DispatchPassThrough;
	}

	driver->MajorFunction[IRP_MJ_READ] = DispatchRead;

	KdPrint(("[ KFD ]\t load was successful\r\n"));

	status = AttachDevice(driver);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("[ KFD ]\t attach device was failed\r\n"));
		return status;
	}
	else
	{
		KdPrint(("[ KFD ]\t attach device was successful\r\n"));
	}

	return status;
}