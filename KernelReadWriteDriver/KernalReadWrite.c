#pragma warning (disable : 4100)

#include "KernalReadWrite.h"
#include "messages.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	
	UNREFERENCED_PARAMETER(pRegistryPath);
	
	
	pDriverObject->DriverUnload = UnloadDriver;
	DebugMessage("Welcome to this Kernal Drive");


	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) {
	
	UNREFERENCED_PARAMETER(pDriverObject);
	
	DebugMessage("Goodbyte from this drive ");

	return STATUS_SUCCESS;
}