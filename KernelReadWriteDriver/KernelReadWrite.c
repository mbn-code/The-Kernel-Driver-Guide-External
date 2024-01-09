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
 * License:
 * This code is distributed under [insert license type here]. See the accompanying LICENSE file for more details.
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

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	
	UNREFERENCED_PARAMETER(pRegistryPath);
	
	// Assign Driverens Unload function til UnloadDriver functionen.
	pDriverObject->DriverUnload = UnloadDriver;

	// Log Welcome to this kernel driver
	DebugMessage("Welcome to this Kernal Driver");

	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) {
	
	UNREFERENCED_PARAMETER(pDriverObject);
	
	// log Goodbyte from this driver
	DebugMessage("Goodbyte from this driver");

	return STATUS_SUCCESS;
}