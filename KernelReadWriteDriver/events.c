/**
 * @brief Callback function called when a new image is loaded into a process.
 *
 * This function is registered as a callback routine using the PLOAD_IMAGE_NOTIFY_ROUTINE type.
 * It is called whenever a new image is loaded into a process. In this case, the function checks
 * if the loaded image corresponds to the "ac_client.exe" process. If it does, it retrieves the
 * process ID and the base module address of the loaded image and stores them in the global variables
 * BaseModuleAdress and ProcessId respectively.
 *
 * @param FullImgName A pointer to a UNICODE_STRING structure that contains the full path of the loaded image.
 * @param ProcId The process ID of the process into which the image is loaded.
 * @param ImgInfo A pointer to an IMAGE_INFO structure that contains information about the loaded image.
 * @return STATUS_SUCCESS indicating that the callback was successful.
 */

#pragma warning (disable : 4047 4024 4311 4189)

#include "Events.h"
#include "messages.h"
#include "Data.h"

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCB(PUNICODE_STRING FullImgName, HANDLE ProcId, PIMAGE_INFO ImgInfo)
{
	// Check if the loaded image corresponds to the "ac_client.exe" process
	if (wcsstr(FullImgName->Buffer, L"ac_client.exe"))
	{
		//DebugMessage("Image Found");
		DebugMessage("Process Id Found: %d", ImgInfo->ImageBase);
		DebugMessage("BaseModuleAdress Found: %d", (ULONG)ProcId);

		// Store the base module address and the process ID in global variables
		BaseModuleAdress = ImgInfo->ImageBase;
		ProcessId = (ULONG)ProcId;
	}

	return STATUS_SUCCESS;
}
