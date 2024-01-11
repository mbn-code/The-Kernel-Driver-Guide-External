#pragma warning (disable : 4047 4024)

#include "Events.h"
#include "messages.h"
#include "Data.h"

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCB(PUNICODE_STRING FullImgName, HANDLE ProcId, PIMAGE_INFO ImgInfo)
{
	if (wcsstr(FullImgName->Buffer, L"ac_client.exe"))
	{
		DebugMessage("Image Found");

		BaseModuleAdress = ImgInfo->ImageBase;

		DebugMessage("Image Process ID: %d \n", ProcId);

		DebugMessage("Base Module Adress: %d \n", BaseModuleAdress);
	}

	return STATUS_SUCCESS;
}
