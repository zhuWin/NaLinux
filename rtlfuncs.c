#include "precomp.h"

HANDLE hEvent;
WCHAR DisplayBuffer[1024];
USHORT LinePos = 0;
WCHAR PutChar[2] = L" ";
UNICODE_STRING CharString = {2, 2, PutChar};

NTSTATUS
__cdecl
RtlCliDisplayString(IN PCH Message, ...)
{
    va_list MessageList;
    PCHAR MessageBuffer;
    UNICODE_STRING MessageString;
    NTSTATUS Status;

    //
    // Allocate Memory for the String Buffer
    //
    MessageBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, 512);

    //
    // First, combine the message
    //
    va_start(MessageList, Message);
    _vsnprintf(MessageBuffer, 512, Message, MessageList);
    va_end(MessageList);

    //
    // Now make it a unicode string
    //
    RtlCreateUnicodeStringFromAsciiz(&MessageString, MessageBuffer);

    //
    // Display it on screen
    //
    Status = RtlCliPrintString(&MessageString);

    //
    // Free Memory
    //
    RtlFreeHeap(RtlGetProcessHeap(), 0, MessageBuffer);
    RtlFreeUnicodeString(&MessageString);

    //
    // Return to the caller
    //
    return Status;
}

NTSTATUS
RtlCliPrintString(IN PUNICODE_STRING Message)
{
    NTSTATUS Status;
    ULONG i;

    //
    // Loop every character
    //
    for (i = 0; i < (Message->Length / sizeof(WCHAR)); i++)
    {
        //
        // Print the character
        //
        Status = RtlCliPutChar(Message->Buffer[i]);
    }

    //
    // Return status
    //
    return Status;
}

NTSTATUS
RtlCliPutChar(IN WCHAR Char)
{
    //
    // Initialize the string
    //
    CharString.Buffer[0] = Char;

    //
    // Check for overflow, or simply update.
    //
#if 0
    if (LinePos++ > 80)
    {
        //
        // We'll be on a new line. Do the math and see how far.
        //
        MessageLength = NewPos - 80;
        LinePos = sizeof(WCHAR);
    }
#endif

    //
    // Make sure that this isn't backspace
    //
    if (Char != '\r')
    {
        //
        // Check if it's a new line
        //
        if (Char == '\n')
        {
            //
            // Reset the display buffer
            //
            LinePos = 0;
            DisplayBuffer[LinePos] = UNICODE_NULL;
        }
        else
        {
            //
            // Add the character in our buffer
            //
            DisplayBuffer[LinePos] = Char;
            LinePos++;
        }
    }

    //
    // Print the character
    //
    return NtDisplayString(&CharString);
}

NTSTATUS
RtlClipBackspace(VOID)
{
    UNICODE_STRING BackString;

    //
    // Update the line position
    //
    LinePos--;

    //
    // Finalize this buffer and make it unicode
    //
    DisplayBuffer[LinePos] = ANSI_NULL;
    RtlInitUnicodeString(&BackString, DisplayBuffer);

    //
    // Display the buffer
    //
    return NtDisplayString(&BackString);
}

// Stub for Windows 2000
#define __declspec(x) // temporarily disable __declspec so you can declare function freely

// This causes BSOD in Windows 2000 due of its import being missing so stub it
NTSYSAPI LONG NTAPI RtlUnhandledExceptionFilter(IN struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    return EXCEPTION_CONTINUE_SEARCH;
}

#undef __declspec // restore __declspec macro to normal

NTSTATUS
RtlCliOpenAllInputDevices(OUT HANDLE* KeyboardHandles, IN CON_DEVICE_TYPE Type)
{
    WCHAR deviceName[64];
    UNICODE_STRING Driver;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK Iosb;
    NTSTATUS Status;
    HANDLE hDriver;
    ULONG count = 0;
    ULONG i;

    for (i = 0; i < 64 /* MAX_KEYBOARDS */; ++i)
    {
        if (Type == KeyboardType) {
	        swprintf(deviceName, L"\\Device\\KeyboardClass%lu", i);
            RtlInitUnicodeString(&Driver, deviceName);
        }

        InitializeObjectAttributes(&ObjectAttributes, &Driver, OBJ_CASE_INSENSITIVE, NULL, NULL);

        Status = NtCreateFile(
            &hDriver,
            SYNCHRONIZE | GENERIC_READ | FILE_READ_ATTRIBUTES,
            &ObjectAttributes,
            &Iosb,
            NULL,
            FILE_ATTRIBUTE_NORMAL,
            0,
            FILE_OPEN,
            FILE_DIRECTORY_FILE,
            NULL,
            0
        );

        if (NT_SUCCESS(Status))
        {
            KeyboardHandles[i] = hDriver;
            count++;
        } else {
            continue;
        }
    }

    return count > 0 ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
}

/*NTSTATUS
RtlCliOpenInputDevice(OUT PHANDLE Handle,
    IN CON_DEVICE_TYPE Type)
{
    UNICODE_STRING Driver;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK Iosb;
    HANDLE hDriver;
    NTSTATUS Status;

    //
    // Chose the driver to use
    // FIXME: Support MouseType later
    // FIXME: Don't hardcode keyboard path
    //
    if (Type == KeyboardType)
    {
        RtlInitUnicodeString(&Driver, L"\\Device\\KeyboardClass0");
    }

    //
    // Initialize the object attributes
    //
    InitializeObjectAttributes(&ObjectAttributes,
        &Driver,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL);

    //
    // Open a handle to it
    //
    Status = NtCreateFile(&hDriver,
        SYNCHRONIZE | GENERIC_READ | FILE_READ_ATTRIBUTES,
        &ObjectAttributes,
        &Iosb,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_OPEN,
        FILE_DIRECTORY_FILE,
        NULL,
        0);

    //
    // Now create an event that will be used to wait on the device
    //
    InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL);
    Status = NtCreateEvent(&hEvent, EVENT_ALL_ACCESS, &ObjectAttributes, 1, 0);

    //
    // Return the handle
    //
    *Handle = hDriver;
    return Status;
}*/

NTSTATUS
RtlCliShutdown(VOID)
{
    BOOLEAN Old;

    //
    // Get the shutdown privilege and shutdown the system
    //
    RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &Old);
    return ZwShutdownSystem(ShutdownNoReboot);
}

NTSTATUS
RtlCliReboot(VOID)
{
    BOOLEAN Old;

    //
    // Get the shutdown privilege and shutdown the system
    //
    RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &Old);
    return ZwShutdownSystem(ShutdownReboot);
}
