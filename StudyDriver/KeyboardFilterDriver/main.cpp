#include "stdafx.h"

PDEVICE_OBJECT keyboard_device = nullptr;
ULONG pending_key = 0;

VOID Unload(PDRIVER_OBJECT driver)
{
	LARGE_INTEGER interval = { 0 };
	PDEVICE_OBJECT device = driver->DeviceObject;

	interval.QuadPart = TIME_SECONDS(1);

	IoDetachDevice(((PDEVICE_EXTENSION)device->DeviceExtension)->lower_keyboard_device);

	while (pending_key)
	{
		KdPrint(("[ KFD ]\t remain pending key(%lu)\r\n"), pending_key);
		KeDelayExecutionThread(KernelMode, false, &interval);
	}

	IoDeleteDevice(keyboard_device);
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

	IoCopyCurrentIrpStackLocationToNext(irp);

	status = IoCallDriver(((PDEVICE_EXTENSION)device->DeviceExtension)->lower_keyboard_device, irp);

	return status;
}

NTSTATUS ReadComplete(PDEVICE_OBJECT device, PIRP irp, PVOID context)
{
	LPTSTR key_flag[4] = { "keydown", "keyup", "E0", "E1" };
	PKEYBOARD_INPUT_DATA key = (PKEYBOARD_INPUT_DATA)irp->AssociatedIrp.SystemBuffer;

	int struct_num = irp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);

	if (NT_SUCCESS(irp->IoStatus.Status))
	{
		for (int i = 0; i < struct_num; i++)
		{
			KdPrint(("the scan code was %x (%s)\n", key[i].MakeCode, key_flag[key->Flags]));
		}
	}

	if (irp->PendingReturned)
	{
		IoMarkIrpPending(irp);
	}

	pending_key--;

	return irp->IoStatus.Status;
}

NTSTATUS DispatchRead(PDEVICE_OBJECT device, PIRP irp)
{
	IoCopyCurrentIrpStackLocationToNext(irp);

	IoSetCompletionRoutine(irp, ReadComplete, nullptr, true, true, true);

	pending_key++;

	return IoCallDriver(((PDEVICE_EXTENSION)device->DeviceExtension)->lower_keyboard_device, irp);
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