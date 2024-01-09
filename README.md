# The-Kernel-Driver-Tutorial
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


