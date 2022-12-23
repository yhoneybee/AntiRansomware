#include "stdafx.h"

PDEVICE_OBJECT mouse_device = nullptr;
PDEVICE_OBJECT mouse_filter_device = nullptr;
PFILE_OBJECT mouse_file_object = nullptr;
ULONG pending_key = 0;

VOID Unload(PDRIVER_OBJECT driver)
{
	LARGE_INTEGER interval = { 0 };
	PDEVICE_OBJECT device = driver->DeviceObject;

	interval.QuadPart = TIME_SECONDS(1);

	IoDetachDevice(((PDEVICE_EXTENSION)device->DeviceExtension)->lower_device);

	while (pending_key)
	{
		KdPrint(("[ KFD ]\t remain pending key(%lu)\r\n"), pending_key);
		KeDelayExecutionThread(KernelMode, false, &interval);
	}

	IoDeleteDevice(mouse_device);
	KdPrint(("[ KFD ]\t unload was successful\r\n"));
}

NTSTATUS AddDevice(PDRIVER_OBJECT driver, PDEVICE_OBJECT physical_device)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING target_device = RTL_CONSTANT_STRING(L"\\Device\\PointerClass0");

	status = IoCreateDevice(driver, sizeof(DEVICE_EXTENSION), nullptr, FILE_DEVICE_MOUSE, 0, false, &mouse_filter_device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("[ MFD ]\t IoCreateDevice was fail"));
		return status;
	}

	mouse_device->Flags |= DO_BUFFERED_IO;

	status = IoGetDeviceObjectPointer(&target_device, STANDARD_RIGHTS_ALL, &mouse_file_object, &mouse_device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("[ MFD ]\t IoGetDeviceObjectPointer was fail"));
		IoDeleteDevice(mouse_filter_device);
		return status;
	}

	mouse_device = IoAttachDeviceToDeviceStack(mouse_filter_device, physical_device);
	if (mouse_device == nullptr)
	{
		KdPrint(("[ MFD ]\t IoAttachDeviceToDeviceStack was fail"));
	}

	KdPrint(("[ MFD ]\t connect mouse device successful"));

	return status;
}

NTSTATUS DispatchGeneral(PDEVICE_OBJECT device, PIRP irp)
{
	PIO_STACK_LOCATION current_irp_stack = IoGetCurrentIrpStackLocation(irp);
	PIO_STACK_LOCATION next_irp_stack = IoGetNextIrpStackLocation(irp);

	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;

	if (device == mouse_filter_device)
	{
		*next_irp_stack = *current_irp_stack;
		return IoCallDriver(mouse_device, irp);
	}

	return STATUS_SUCCESS;
}

NTSTATUS ReadComplete(PDEVICE_OBJECT device, PIRP irp, PVOID context)
{
	PIO_STACK_LOCATION io_stack_location = IoGetCurrentIrpStackLocation(irp);
	PMOUSE_INPUT_DATA mouse_data;

	if (!NT_SUCCESS(irp->IoStatus.Status))
	{
		return irp->IoStatus.Status;
	}

	mouse_data = (PMOUSE_INPUT_DATA)irp->AssociatedIrp.SystemBuffer;

	if (mouse_data->Flags & MOUSE_MOVE_ABSOLUTE)
	{
		KdPrint(("[ MFD ]\t ABS: X = %ld / Y = %ld"), mouse_data->LastX, mouse_data->LastY);
	}
	else
	{
		KdPrint(("[ MFD ]\t X = %ld / Y = %ld"), mouse_data->LastX, mouse_data->LastY);
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
	pending_key++;

	PIO_STACK_LOCATION current_irp_stack = IoGetCurrentIrpStackLocation(irp);
	PIO_STACK_LOCATION next_irp_stack = IoGetNextIrpStackLocation(irp);

	*next_irp_stack = *current_irp_stack;

	IoSetCompletionRoutine(irp, ReadComplete, device, true, true, true);

	return IoCallDriver(mouse_device, irp);
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING registry_path)
{
	NTSTATUS status = STATUS_SUCCESS;

	driver->DriverUnload = Unload;
	driver->DriverExtension->AddDevice = AddDevice;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		driver->MajorFunction[i] = DispatchGeneral;
	}

	driver->MajorFunction[IRP_MJ_READ] = DispatchRead;

	KdPrint(("[ MFD ]\t load was successful\r\n"));

	if (!NT_SUCCESS(status))
	{
		KdPrint(("[ MFD ]\t attach device was failed\r\n"));
		return status;
	}
	else
	{
		KdPrint(("[ MFD ]\t attach device was successful\r\n"));
	}

	return status;
}