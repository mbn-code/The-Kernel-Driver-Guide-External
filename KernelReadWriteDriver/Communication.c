/**
 * @brief Håndterer IOCTL-forespørgsler, der sendes til enhedsdriveren.
 *
 * Denne funktion er ansvarlig for at håndtere forskellige IOCTL-forespørgsler, der sendes til enhedsdriveren.
 * Den udfører forskellige operationer baseret på kontrolkoden angivet i IOCTL-forespørgslen.
 *
 * @param DeviceObject Pegeren til enhedsobjektet.
 * @param Irp Pegeren til IRP (I/O Request Packet), der repræsenterer IOCTL-forespørgslen.
 * @return NTSTATUS Statussen for håndtering af IOCTL-forespørgslen.
 */
NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	// Implementering af funktionen
}

/**
 * @brief Håndterer lukkeopkaldsforespørgslen, der sendes til enhedsdriveren.
 *
 * Denne funktion er ansvarlig for at håndtere lukkeopkaldsforespørgslen, der sendes til enhedsdriveren.
 * Den sætter statussen for IRP til succes og fuldfører forespørgslen.
 *
 * @param DeviceObject Pegeren til enhedsobjektet.
 * @param Irp Pegeren til IRP (I/O Request Packet), der repræsenterer lukkeopkaldsforespørgslen.
 * @return NTSTATUS Statussen for håndtering af lukkeopkaldsforespørgslen.
 */
NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	// Implementering af funktionen
}

/**
 * @brief Håndterer oprettelsesopkaldsforespørgslen, der sendes til enhedsdriveren.
 *
 * Denne funktion er ansvarlig for at håndtere oprettelsesopkaldsforespørgslen, der sendes til enhedsdriveren.
 * Den sætter statussen for IRP til succes og fuldfører forespørgslen.
 *
 * @param DeviceObject Pegeren til enhedsobjektet.
 * @param Irp Pegeren til IRP (I/O Request Packet), der repræsenterer oprettelsesopkaldsforespørgslen.
 * @return NTSTATUS Statussen for håndtering af oprettelsesopkaldsforespørgslen.
 */

#pragma warning (disable : 4100 4047 4024 4022)

#include "Communication.h"
#include "messages.h"
#include "Data.h"
#include "Events.h"
#include "memory.h"

// Definerer funktionen IoControl, der håndterer IO-kontrol kald.
NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    // Ignorerer uanvendte parametre.
    UNREFERENCED_PARAMETER(DeviceObject);

    // Initialiserer variabler.
    NTSTATUS CurrentStatus = STATUS_UNSUCCESSFUL;
    ULONG ByteIO = 0;

    // Henter information om IRP-stakken.
    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);

    // Henter kontrolkoden for IO-kontrol.
    ULONG ControlCode = Stack->Parameters.DeviceIoControl.IoControlCode;

    // Håndterer forskellige kontrolkoder.
    if (ControlCode == IO_GET_CLIENTADRESS) 
    {
        // Henter output-bufferen.
        PULONG OutPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;

        // Skriver besked til debugging.
        DebugMessage("ClientAdress: %d", BaseModuleAdress);

        // Gemmer adressen i output-bufferen.
        *OutPut = BaseModuleAdress;

        // Opdaterer antallet af bytes læst/skrevet.
        ByteIO = sizeof(*OutPut);

        // Opdaterer statussen til succes.
        CurrentStatus = STATUS_SUCCESS;
    }
    else if (ControlCode == IO_GET_PROCESSID)
    {
        // Henter output-bufferen.
        PULONG OutPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;

        // Skriver besked til debugging.
        DebugMessage("ProcessId: %d", ProcessId);

        // Gemmer process-id i output-bufferen.
        *OutPut = ProcessId;

        // Opdaterer antallet af bytes læst/skrevet.
        ByteIO = sizeof(*OutPut);

        // Opdaterer statussen til succes.
        CurrentStatus = STATUS_SUCCESS;
    }
    else if (ControlCode == IO_SET_IMAGEBUFFER) 
    {
        // Henter den nye billedbuffer.
        wchar_t *NewImgBuffer = (wchar_t*)Irp->AssociatedIrp.SystemBuffer;

        // Opdaterer billedbufferen.
        ImageBuffer = NewImgBuffer;

        // Opdaterer statussen til succes.
        CurrentStatus = STATUS_SUCCESS;
    }
    else if (ControlCode == IO_READ_REQUEST)
    {
        // Henter input-data til læseanmodningen.
        PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
        PEPROCESS Process;

        // Slår processen op baseret på dens id.
        if (NT_SUCCESS(PsLookupProcessByProcessId(ReadInput->ProcessId, &Process)))
        {
            // Læser virtuel hukommelse fra processen.
            KernelReadVirtualMemory(Process, ReadInput->Address, ReadInput->pBuff, ReadInput->Size);

            // Opdaterer statussen til succes.
            CurrentStatus = STATUS_SUCCESS;

            // Opdaterer antallet af bytes læst/skrevet.
            ByteIO = sizeof(KERNEL_READ_REQUEST);
        }
    }
    else if (ControlCode == IO_WRITE_REQUEST)
    {
        // Henter input-data til skriveanmodningen.
        PKERNEL_WRITE_REQUEST WriteInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
        PEPROCESS Process;

        // Slår processen op baseret på dens id.
        if (NT_SUCCESS(PsLookupProcessByProcessId(WriteInput->ProcessId, &Process)))
        {
            // Skriver til virtuel hukommelse i processen.
            KernelWriteVirtualMemory(Process, WriteInput->pBuff, WriteInput->Address, WriteInput->Size);

            // Opdaterer statussen til succes.
            CurrentStatus = STATUS_SUCCESS;

            // Opdaterer antallet af bytes læst/skrevet.
            ByteIO = sizeof(KERNEL_READ_REQUEST);
        }
    }
    else 
    {
        // Hvis kontrolkoden ikke genkendes, sættes ByteIO til 0.
        ByteIO = 0;
    }

    // Opdaterer status og antal bytes for IRP'en.
    Irp->IoStatus.Status = CurrentStatus;
    Irp->IoStatus.Information = ByteIO;

    // Fuldfører IRP'en og frigiver ressourcer.
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    // Returnerer statussen for operationen.
    return CurrentStatus;
}

// Definerer funktionen CloseCall, der kaldes når en forbindelse lukkes.
NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    // Ignorerer uanvendte parametre.
    UNREFERENCED_PARAMETER(DeviceObject);

    // Sætter statussen til succes.
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    // Fuldfører IRP'en og frigiver ressourcer.
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    // Skriver besked til debugging.
    DebugMessage("Forbindelse afsluttet\n");

    // Returnerer statussen for operationen.
    return STATUS_SUCCESS;
}

// Definerer funktionen CreateCall, der kaldes når en forbindelse oprettes.
NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    // Ignorerer uanvendte parametre.
    UNREFERENCED_PARAMETER(DeviceObject);

    // Sætter statussen til succes.
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    // Fuldfører IRP'en og frigiver ressourcer.
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    // Skriver besked til debugging.
    DebugMessage("Forbindelse etableret\n");

    // Returnerer statussen for operationen.
    return STATUS_SUCCESS;
}
