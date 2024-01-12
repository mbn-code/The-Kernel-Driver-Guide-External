#include <iostream>
#include "Kernelinterface.hpp"
#include "Util.hpp"
#include <SFML/Graphics.hpp>>

int main()
{
    KernelInterface Driver = KernelInterface("\\\\.\\RWDriver");

    ULONG BaseModuleAdress = Driver.GetClientAdress();
    ULONG ProcessId = Driver.GetProcessId();

    std::cout << "ac_client.exe Base Adress: " << std::hex << BaseModuleAdress << std::endl;
    std::cout << "ac_client.exe Process ID: " << ProcessId << std::endl;

    int value;

    std::cout << "What should the value be?: " << std::endl;
    std::cin >> value;

    uint32_t LocalPlayerAdress = Driver.ReadVirtualMemory<uint32_t>(ProcessId, BaseModuleAdress + 0x17E0A8, sizeof(uint32_t));

    Driver.WriteVirtualMemory(ProcessId, LocalPlayerAdress + 0xEC, value, sizeof(value));
}
