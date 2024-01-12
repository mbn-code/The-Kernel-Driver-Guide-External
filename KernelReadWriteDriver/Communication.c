#pragma warning (disable : 4100 4047 4024 4022)

#include "Communication.h"
#include "messages.h"
#include "Data.h"
#include "Events.h"
#include "memory.h"

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS CurrentStatus = STATUS_UNSUCCESSFUL;
	ULONG ByteIO = 0;

	PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);

	ULONG ControlCode = Stack->Parameters.DeviceIoControl.IoControlCode;

	if (ControlCode == IO_GET_CLIENTADRESS) 
	{
		PULONG OutPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*OutPut = BaseModuleAdress;

		ByteIO = sizeof(*OutPut);
		CurrentStatus = STATUS_SUCCESS;
	}
	else if (ControlCode == IO_GET_PROCESSID)
	{
		PULONG OutPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*OutPut = ProcessId;

		ByteIO = sizeof(*OutPut);
		CurrentStatus = STATUS_SUCCESS;
	}
	else if (ControlCode == IO_SET_IMAGEBUFFER) 
	{
		wchar_t *NewImgBuffer = (wchar_t*)Irp->AssociatedIrp.SystemBuffer;

		//size_t Size = wcslen(ImageBuffer);

		//RtlCopyMemory(ImageBuffer, NewImgBuffer, Size * sizeof(wchar_t));

		//DebugMessage("SETUP NewImgBuffer contains this: %ls \n", NewImgBuffer);

		ImageBuffer = NewImgBuffer;

		//DebugMessage("SETUP ImageBuffer contains this: %ls \n", ImageBuffer);

		CurrentStatus = STATUS_SUCCESS;
	}
	else if (ControlCode == IO_READ_REQUEST)
	{
		PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;

		if (NT_SUCCESS(PsLookupProcessByProcessId(ReadInput->ProcessId, &Process)))
		{
			KernelReadVirtualMemory(Process, ReadInput->Address, ReadInput->pBuff, ReadInput->Size);
			CurrentStatus = STATUS_SUCCESS;
			ByteIO = sizeof(KERNEL_READ_REQUEST);
		}
	}
	else if (ControlCode == IO_WRITE_REQUEST)
	{
		PKERNEL_WRITE_REQUEST WriteInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;

		DebugMessage("Set Value to: &d", WriteInput->pBuff)

		if (NT_SUCCESS(PsLookupProcessByProcessId(WriteInput->ProcessId, &Process)))
		{
			KernelWriteVirtualMemory(Process, WriteInput->pBuff, WriteInput->Address, WriteInput->Size);
			CurrentStatus = STATUS_SUCCESS;
			ByteIO = sizeof(KERNEL_READ_REQUEST);
		}
	}
	else 
	{
		ByteIO = 0;
	}

	Irp->IoStatus.Status = CurrentStatus;
	Irp->IoStatus.Information = ByteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return CurrentStatus;
}


NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	// Set Status to Success
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	DebugMessage("Connection Terminated\n");

	return STATUS_SUCCESS;
}


NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	// Set Status to Success
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	DebugMessage("Connection Established\n");

	return STATUS_SUCCESS;
}