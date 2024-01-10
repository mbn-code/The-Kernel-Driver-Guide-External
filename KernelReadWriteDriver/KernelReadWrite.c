/*
 * RWDriver - Kernel Read/Write Driver
 * Author: Oskar Stefanski & Malthe Bang Norengaard
 * Language: C (Kernel Driver), C++ (User Mode application)
 *
 * Description:
 * This kernel driver, written in C, is designed for read and write operations. The associated User Mode application will be written in C++.
 *
 * Disclaimer:
 * This code is provided as-is, without any warranty or support. Use it at your own risk. The authors are not responsible for any damage or loss resulting from the use of this code.
 *
 * Notes:
 * - Disable ligegyldig error --- Da det er en driver will alle warnings blive treated som errors for at undgå BSOD f.eks.
 * - To build the kernel service, use the command: sc create RWDriver type= kernel binpath="<PathTilDriverBuild>\RWDriver.sys"
 * - Ensure SecureBoot is disabled in BIOS if there are issues.
 * - To allow the driver, run: bcdedit /set testsigning on
 * - Start/stop the driver with: sc stop/start RWDriver
 */


#pragma warning (disable : 4100)

#include "KernelReadWrite.h"
#include "messages.h"

#define IOCTL_READ_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	
	UNREFERENCED_PARAMETER(pRegistryPath);
	
	// Point pDriverObjects DriverUnload pointer til UnloadDriver
	pDriverObject->DriverUnload = UnloadDriver;

	// Log Welcome to this kernel driver
	DebugMessage("Kernel Driver Started Successfully");

	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) {
	
	UNREFERENCED_PARAMETER(pDriverObject);
	
	// log Goodbyte from this driver
	DebugMessage("Goodbyte from this driver");

	PsRemoveLoadImageNotifyRoutine()

	return STATUS_SUCCESS;
}


// TOOD : Implement IOCTL for communication between user-mode and kernel-driver
