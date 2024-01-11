#include "events.h"
#include "messages.h"

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING, HANDLE ProcessId, PIMAGE_INFO ImageInfo) {
	DebugMessage ("Image Loaded: %ls \n", FullImageName->Buffer)

	if (wcsstr(FullImageName->Buffer))

}