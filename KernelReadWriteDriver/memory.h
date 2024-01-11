#pragma once
#pragma warning (disable : 4047 4024 4022)
#include "ntifs.h"

NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS SourceProcess,
	PVOID SourceAdress,
	PEPROCESS TargetProcess,
	PVOID TargetAdress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode, // KernelMode for Kernel
	PSIZE_T ReturnSize
);

NTSTATUS KernelReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAdress, SIZE_T Size);
NTSTATUS KernelWriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAdress, SIZE_T Size);