#pragma once

#include "Communications.hpp"

class KernelInterface
{
public:
	HANDLE hDriver;

	KernelInterface(LPCSTR RegistryPath) {
		hDriver = CreateFileA(RegistryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	DWORD GetClientAdress() 
	{
		if (hDriver == INVALID_HANDLE_VALUE) 
		{
			return 0;
		}

		ULONG ReturnAdress;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_CLIENTADRESS, &ReturnAdress, sizeof(ReturnAdress), &ReturnAdress, sizeof(ReturnAdress), &Bytes, NULL)) {
			return ReturnAdress;
		}

		return 0;
	}
};