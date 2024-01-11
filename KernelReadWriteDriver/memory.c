#pragma warning (disable : 4047 4024 4022)

#include "memory.h"

NTSTATUS KernelReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAdress, SIZE_T Size)
{
	PSIZE_T Bytes;

	return MmCopyVirtualMemory(Process, SourceAddress, PsGetCurrentProcess(), TargetAdress, Size, KernelMode, &Bytes);
}

NTSTATUS KernelWriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAdress, SIZE_T Size)
{
	PSIZE_T Bytes;

	return MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, Process, TargetAdress, Size, KernelMode, &Bytes);
}