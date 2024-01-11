#pragma warning (disable : 4047 4024 4311)

#include "Events.h"
#include "messages.h"
#include "Data.h"

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCB(PUNICODE_STRING FullImgName, HANDLE ProcId, PIMAGE_INFO ImgInfo)
{
	if (wcsstr(FullImgName->Buffer, ImageBuffer))
	{
		DebugMessage("Image Found");
		DebugMessage("Buffer: %ls", FullImgName->Buffer);

		BaseModuleAdress = ImgInfo->ImageBase;
		ProcessId = (ULONG)ProcId;
	}

	return STATUS_SUCCESS;
}
