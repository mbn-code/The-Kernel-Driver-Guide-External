#pragma warning (disable : 4047 4024 4311 4189)

#include "Events.h"
#include "messages.h"
#include "Data.h"

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCB(PUNICODE_STRING FullImgName, HANDLE ProcId, PIMAGE_INFO ImgInfo)
{
	if (wcsstr(FullImgName->Buffer, L"ac_client.exe"))
	{
		//DebugMessage("Image Found");
		DebugMessage("Process Id Found: %d", ImgInfo->ImageBase);
		DebugMessage("BaseModuleAdress Found: %d", (ULONG)ProcId);

		BaseModuleAdress = ImgInfo->ImageBase;
		ProcessId = (ULONG)ProcId;
	}

	return STATUS_SUCCESS;
}
