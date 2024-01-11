    #include <iostream>
#include "Kernelinterface.hpp"
#include "Util.hpp"

int main()
{
    KernelInterface Driver = KernelInterface("\\\\.\\RWDriver");

    if (Driver.SetImageBuffer(const_cast<wchar_t*>(L"ac_client.exe")))
    {
        std::string S;

        while (true) 
        {

            std::cin >> S;

            if (S == "1") {
                ULONG BaseModuleAdress = Driver.GetClientAdress();

                std::cout << "ac_client.exe Base Adress: " << BaseModuleAdress << std::endl;
            }
        }
    }
}
