![banner](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/998669c2-d062-4ce0-a671-2917e84058ca)

# The-Kernel-Driver-GUIDE.

Kernal Driver Tutorial For Getting Started With Kernel Drivers

# Hello
Hello and welcome to this GUIDE, remember to read everything carefully to not make any mistakes.
This is a simple guide for getting started with Kernel Drivers. 
This was created to make an easy follow along guide on creating a simple kernel driver for windows. 

# Prerequisites

Before you begin, you will need to make sure you have the following:

- A Windows machine with Secure Boot Disabled
- Windows Machine With at least 4 GB RAM

# Disabling Secure Boot

- a. Restart your computer: Restart your computer and keep pressing the F2, F10, or Delete key (depending on your computer's manufacturer) until you enter the BIOS/UEFI settings.

- b. Locte The Seucre Boot Feature: Once you have entered the BIOS, look for a section called "Security" or "Trusted Platform Module (TPM)". The Secure Boot setting should be within this section. If not, search for you rspecific manufacturer of motherboard. 

- c. Save Settings: Once Secure Boot is disabled, save and exit BIOS and boot into windows. 

# Additional tips for disabling Secure Boot:

- Back up your data before disabling Secure Boot.

- Disable Secure Boot only when you need to run a virtual machine or other software that requires it.

- Once you have finished using the virtual machine or software that requires Secure Boot to be disabled, re-enable Secure Boot to protect your computer.

# Download and Setup

When we want to develop a kernel driver we first need to setup our Enviroment.
We need to download a few requirements, which are listed down below:

 * Tip1: Remember to install "WDK" after installing Visual Studio 2022 as WDK is an extension to VS22
 * Tip2: Remember to Install all Individual Components when installing Visual Studio 2022

Step 1: Install Visual Studio 2022	Link: https://visualstudio.microsoft.com/downloads/

Step 2: Install SDK	Link: https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/

Step 3: Install WDK	Link: https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk

- a1. Follow This Tutorial For Installing Windows Driver Kit if the tutorial above is outdated: https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk
- a2. When installing components for Visual Studio 2022 need to select "Desktop development with C++" with these individual components:
  
  - C++ ATL for latest v143 build tools with Spectre Mitigations (ARM64/ARM64EC)
  - C++ ATL for latest v143 build tools with Spectre Mitigations (x86 & x86)
  - C++ MFC for latest v143 build tools with Spectre Mitigations (ARM64/ARM64EC)
  - MSVC v143 - VS 2022 C++ ARM64/ARM64EC Spectre-mitigated libs (Latest)
  - MSVC v143 - VS 2022 C++ x64/x86 Spectre-mitigated libs (Latest)

- **Installing Debug View**
- * Download Debug View from this direct link: https://download.sysinternals.com/files/DebugView.zip
  * Or go here: https://learn.microsoft.com/en-us/sysinternals/downloads/debugview


# Visual Studio 2022 Project Setup 
  - 1 Create a new kernel mode driver project in Visual Studio. ( Kernel Mode Driver, Empty (KMDF).

  ![9](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/51d10f13-6859-4d0b-876c-084859c57651)




    Right click on the project
    
  ![5](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/5d1dd955-ca81-4faf-be77-d41adca4c1ec)
  
  - 2 Activate "Use Local Time" and Enable Active(Release)
  
  ![6](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/44acad7f-24b8-4fdd-b756-b9631a7b4f61)

  - 3 Navigate to "Linker" -> "Command Line" and write

    "/INTEGRITYCHECK"

  - 4 Add A Class

    Tip: Name this the same name as the project
    
    Right Click on Properties -> Hover on Add -> Click On Class
    ![8](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/ec977ceb-d804-4473-9b13-9b9bbc453c77)

  
# Creating And Running The Driver ( Using Debug View ) 

- 2 Add a new header file named "messages.h" ( In the Header Files Folder ).
- 3 Define a function called debug_message that takes a string and some extra parameters.
  ![1](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/b111f975-b64c-4655-86a2-99a4e0c8a54c)

   
- 4 Include the "messages.h" header file in your source file.

  ![2](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/84ad28b7-291d-4f9e-93b4-873f4d95e5e0)

- 5.1 Create a function called driver_entry that takes a PDRIVER_OBJECT and a PUNICODE_STRING.
- 5.2 Create a function called unload_driver that takes a PDRIVER_OBJECT.
- 5.3 Call the debug_message function to print a message when the driver starts and stops.
  ![3](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/a4a6cdf2-47d2-48a5-8870-1c414499f0ea)

- 8 Compile the driver.

- 9 Set Kernal driver Bin Path using sc create command.
  tip: Assuming you have the project on C drive, just replace the "computer name" with your actual computer name, and "KernalReadWriteDriver.sys" With the name if your .sys file. 

      sc create KernelReadWriteDriver type= Kernel Binpath="C:\Users\computer name\source\repos\KernelReadWriteDriver\x64\Release\KernelReadWriteDriver.sys"
  
- 10 Enable test signing
  Using the cmd enable testsigning with the following command: 

      bcdedit /set testsigning on

- 11 Start DebugView in ADMIN mode
  
- 12 Enable Capture Kernel In Debug View
  ![4](https://github.com/CollinEdward/The-Kernel-Driver-Tutorial/assets/66748817/e00cb8ac-d070-43bd-b964-d3837a1b289e)

- 13.1 Load the driver using the sc command.
    Without the quotation marks and with the name of your kernel driver 

      sc start "kernel driver name"

- 13.2 If you get an error when trying to start
    If you get an error when trying to start the service, try to enter this in to ADMIN cmd and restart. 
  
      bcdedit /set nointegritychecks on

- 14 Stop the driver using the sc command.
    Without the quotation marks and with the name of your kernel driver 

      sc stop "kernel driver name"


Congratulations on completing The-Kernel-Driver-Tutorial! You've successfully set up your development environment, disabled Secure Boot, and created a simple kernel driver for Windows.

# Cheat Table Content for Assaulted Cube Process
---

- * Cheat Entries Can be found in this md [cheat-entries.md](cheat-entries.md) or from downloading the [assaultcube.CT](assaultcube.CT) file, and running with cheat engine whilst running the [game](https://github.com/assaultcube/AC/releases/tag/v1.3.0.2).

