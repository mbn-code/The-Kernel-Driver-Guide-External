#include <iostream>
#include "Kernelinterface.hpp"

int main()
{
    KernelInterface Driver = KernelInterface("\\\\.\\RWDriver");

    ULONG BaseModuleAdress = Driver.GetClientAdress();

    std::cout << "ac_client.exe Base Adress: " << BaseModuleAdress << std::endl;

    // Bare lige hold vinduet åbent :>
    while (true) {

    }
}
