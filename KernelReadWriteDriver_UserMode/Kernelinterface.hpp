#pragma once

#include "Communications.hpp"

class KernelInterface
{
public:
	HANDLE hDriver;

	KernelInterface(LPCSTR RegistryPath) {
		hDriver = CreateFileA(RegistryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	bool SetImageBuffer(wchar_t* ImageBuffer)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD Bytes;

		size_t Size = wcslen(ImageBuffer);

		if (DeviceIoControl(hDriver, IO_SET_IMAGEBUFFER, ImageBuffer, Size*sizeof(wchar_t), NULL, NULL, &Bytes, NULL))
		{
			return true;
		}

		return false;
	}

	DWORD GetClientAdress() 
	{
		if (hDriver == INVALID_HANDLE_VALUE) 
		{
			return 0;
		}

		ULONG ReturnAdress;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_CLIENTADRESS, &ReturnAdress, sizeof(ReturnAdress), &ReturnAdress, sizeof(ReturnAdress), &Bytes, NULL)) 
		{
			return ReturnAdress;
		}

		return 0;
	}

	DWORD GetProcessId()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		ULONG ProcessId;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_PROCESSID, &ProcessId, sizeof(ProcessId), &ProcessId, sizeof(ProcessId), &Bytes, NULL))
		{
			return ProcessId;
		}

		return 0;
	}

	template <typename type>
	type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAdress, SIZE_T Size) 
	{
		type Buffer;

		KERNEL_READ_REQUEST ReadReq;
		ReadReq.ProcessId = ProcessId;
		ReadReq.Address = ReadAdress;
		ReadReq.pBuff = &Buffer;
		ReadReq.Size = Size;

		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadReq, sizeof(ReadReq), &ReadReq, sizeof(ReadReq), NULL, NULL))
		{
			return Buffer;
		}

		return Buffer;
	}

	template <typename type>
	bool WriteVirtualMemory(ULONG ProcessId, ULONG WriteAdress, type WriteValue, SIZE_T Size)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD Bytes;

		KERNEL_WRITE_REQUEST WriteReq;
		WriteReq.ProcessId = ProcessId;
		WriteReq.Address = WriteAdress;
		WriteReq.pBuff = &WriteAdress;
		WriteReq.Size = Size;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteReq, sizeof(WriteReq), NULL, NULL, &Bytes, NULL))
		{
			return true;
		}

		return false;
	}
};