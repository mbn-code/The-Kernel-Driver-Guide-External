#pragma warning (disable : 4047 4024 4311 4189)

#include "Events.h"
#include "messages.h"
#include "Data.h"

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCB(PUNICODE_STRING FullImgName, HANDLE ProcId, PIMAGE_INFO ImgInfo)
{
	//("ImageBuffer contains this: %ls \n", ImageBuffer);

	if (wcsstr(FullImgName->Buffer, L"ac_client.exe"))
	{
		//DebugMessage("Image Found");
		//butDebugMessage("Buffer: %ls", FullImgName->Buffer);

		BaseModuleAdress = ImgInfo->ImageBase;
		ProcessId = (ULONG)ProcId;
	}

	return STATUS_SUCCESS;
}
