#pragma once

#include "Communications.hpp"

class KernelInterface
{
public:
    HANDLE hDriver;

    // Constructor
    KernelInterface(LPCSTR RegistryPath) : hDriver(INVALID_HANDLE_VALUE)
    {
        hDriver = CreateFileA(RegistryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
        if (hDriver == INVALID_HANDLE_VALUE)
        {
            // Print the path for debugging
            std::cerr << "Failed to open driver at path: " << RegistryPath << " with error: " << GetLastError() << std::endl;
        }
    }


    // Destructor
    ~KernelInterface()
    {
        if (hDriver != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hDriver);
        }
    }

    // Function to set image buffer
    bool SetImageBuffer(const wchar_t* ImageBuffer)
    {
        if (hDriver == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD Bytes;
        size_t Size = wcslen(ImageBuffer);

        if (DeviceIoControl(hDriver, IO_SET_IMAGEBUFFER, const_cast<wchar_t*>(ImageBuffer), Size * sizeof(wchar_t), nullptr, 0, &Bytes, nullptr))
        {
            return true;
        }

        // Add error handling if necessary
        std::cerr << "SetImageBuffer failed: " << GetLastError() << std::endl;
        return false;
    }

    // Function to get client address
    DWORD GetClientAdress()
    {
        if (hDriver == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        ULONG ReturnAddress;
        DWORD Bytes;

        if (DeviceIoControl(hDriver, IO_GET_CLIENTADRESS, nullptr, 0, &ReturnAddress, sizeof(ReturnAddress), &Bytes, nullptr))
        {
            return ReturnAddress;
        }

        // Add error handling if necessary
        std::cerr << "GetClientAdress failed: " << GetLastError() << std::endl;
        return 0;
    }

    // Function to get process ID
    DWORD GetProcessId()
    {
        if (hDriver == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        ULONG ProcessId;
        DWORD Bytes;

        if (DeviceIoControl(hDriver, IO_GET_PROCESSID, nullptr, 0, &ProcessId, sizeof(ProcessId), &Bytes, nullptr))
        {
            return ProcessId;
        }

        // Add error handling if necessary
        std::cerr << "GetProcessId failed: " << GetLastError() << std::endl;
        return 0;
    }

    // Function to read virtual memory
    template <typename type>
    type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAddress, SIZE_T Size)
    {
        type Buffer = {};

        KERNEL_READ_REQUEST ReadReq;
        ReadReq.ProcessId = ProcessId;
        ReadReq.Address = ReadAddress;
        ReadReq.pBuff = &Buffer;
        ReadReq.Size = Size;

        DWORD Bytes;

        if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadReq, sizeof(ReadReq), &ReadReq, sizeof(ReadReq), &Bytes, nullptr))
        {
            return Buffer;
        }

        // Add error handling if necessary
        std::cerr << "ReadVirtualMemory failed: " << GetLastError() << std::endl;
        return Buffer;
    }

    // Function to write virtual memory
    template <typename type>
    bool WriteVirtualMemory(ULONG ProcessId, ULONG WriteAddress, type WriteValue, SIZE_T Size)
    {
        if (hDriver == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD Bytes;

        KERNEL_WRITE_REQUEST WriteReq;
        WriteReq.ProcessId = ProcessId;
        WriteReq.Address = WriteAddress;
        WriteReq.pBuff = &WriteValue;
        WriteReq.Size = Size;

        if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteReq, sizeof(WriteReq), nullptr, 0, &Bytes, nullptr))
        {
            return true;
        }

        // Add error handling if necessary
        std::cerr << "WriteVirtualMemory failed: " << GetLastError() << std::endl;
        return false;
    }
};
