# The-Kernel-Driver-Automation Script

# Prompt user for confirmation
$confirmation = Read-Host "This script will automate the setup process for The Kernel Driver Guide. Do you want to proceed? (Y/N)"

if ($confirmation -ne 'Y') {
    Write-Host "Setup aborted."
    exit
}

# Download prerequisites
Write-Host "Downloading prerequisites..."
Invoke-WebRequest -Uri "https://download.visualstudio.microsoft.com/download/pr/a061be25-c14a-489a-8c7c-bb72adfb3cab/4DFE83C91124CD542F4222FE2C396CABEAC617BB6F59BDCBDF89FD6F0DF0A32F/VC_redist.x64.exe" -OutFile "VC_redist.x64.exe"
Invoke-WebRequest -Uri "https://download.microsoft.com/download/1/7/1/1718CCC4-6315-4D8E-9543-8E28A4E18C4C/dxwebsetup.exe" -OutFile "dxwebsetup.exe"
Invoke-WebRequest -Uri "https://download.sysinternals.com/files/DebugView.zip" -OutFile "DebugView.zip"

# Install prerequisites
Write-Host "Installing prerequisites..."
Start-Process -FilePath ".\VC_redist.x64.exe" -Wait
Start-Process -FilePath ".\dxwebsetup.exe" -Wait
Expand-Archive -Path ".\DebugView.zip" -DestinationPath ".\DebugView"

# Download and install Visual Studio 2022
Write-Host "Downloading Visual Studio 2022..."
Invoke-WebRequest -Uri "https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=17" -OutFile "vs_community.exe"
Start-Process -FilePath ".\vs_community.exe" -Wait

# Download and install Windows SDK
Write-Host "Downloading Windows SDK..."
Invoke-WebRequest -Uri "https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/" -OutFile "sdksetup.exe"
Start-Process -FilePath ".\sdksetup.exe" -Wait

# Download and install Windows Driver Kit (WDK)
Write-Host "Downloading Windows Driver Kit (WDK)..."
Invoke-WebRequest -Uri "https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk" -OutFile "wdksetup.exe"
Start-Process -FilePath ".\wdksetup.exe" -Wait

# Download Debug View
Write-Host "Downloading Debug View..."
Invoke-WebRequest -Uri "https://download.sysinternals.com/files/DebugView.zip" -OutFile "DebugView.zip"
Expand-Archive -Path ".\DebugView.zip" -DestinationPath ".\DebugView"

# Display completion message
Write-Host "Setup completed successfully!"

# Additional steps
Write-Host "Performing additional steps..."

# Set Kernel driver Bin Path using sc create command
$computerName = Read-Host "Enter your computer name:"
$sysFilePath = "C:\Users\$computerName\source\repos\KernelReadWriteDriver\x64\Release\KernelReadWriteDriver.sys"
sc create KernelReadWriteDriver type= Kernel binPath=$sysFilePath

# Enable test signing
bcdedit /set testsigning on

# Start DebugView in ADMIN mode
Start-Process -FilePath ".\DebugView\Dbgview.exe" -Verb RunAs

# Enable Capture Kernel In Debug View
# Note: This step may need manual intervention as it requires user interaction within DebugView.

# Load the driver using the sc command
$kernelDriverName = Read-Host "Enter the kernel driver name:"
sc start "$kernelDriverName"

# If encountering an error, run the following command in ADMIN cmd and restart
# bcdedit /set nointegritychecks on

# Stop the driver using the sc command
sc stop "$kernelDriverName"

Write-Host "Additional steps completed."
