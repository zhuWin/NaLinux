/*++ NDK Version: 0095

Copyright (c) Alex Ionescu.  All rights reserved.

Header Name:

    umtypes.h

Abstract:

    Type definitions for the basic native types.

Author:

    Alex Ionescu (alex.ionescu@reactos.com)   06-Oct-2004

--*/

#if !defined(_NTDEF_) && !defined(_NTDEF_H)
#define _NTDEF_
#define _NTDEF_H

//
// NDK Applications must use Unicode
//
#ifndef UNICODE
#define UNICODE
#endif

//
// Don't use the SDK status values
//
#ifndef WIN32_NO_STATUS
#define WIN32_NO_STATUS
#endif

//
// Let the NDK know we're in Application Mode
//
#define NTOS_MODE_USER

//
// Dependencies
//
#include <windef.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
#include <winioctl.h>
#include <ntnls.h>

//
// Compiler Definitions
//
#ifndef _MANAGED
#if defined(_M_IX86)
#define FASTCALL                        _fastcall
#else
#define FASTCALL
#endif
#else
#define FASTCALL                        NTAPI
#endif

#if !defined(_M_CEE_PURE)
#define NTAPI_INLINE                    NTAPI
#else
#define NTAPI_INLINE
#endif

//
// Alignment Macros
//
#define ALIGN_DOWN(s, t) \
    ((ULONG)(s) & ~(sizeof(t) - 1))

#define ALIGN_UP(s, t) \
    (ALIGN_DOWN(((ULONG)(s) + sizeof(t) - 1), t))

#define ALIGN_DOWN_POINTER(p, t) \
    ((PVOID)((ULONG_PTR)(p) & ~((ULONG_PTR)sizeof(t) - 1)))

#define ALIGN_UP_POINTER(p, t) \
    (ALIGN_DOWN_POINTER(((ULONG_PTR)(p) + sizeof(t) - 1), t))

//
// Native API Return Value Macros
//
#define NT_SUCCESS(Status)              (((NTSTATUS)(Status)) >= 0)
#define NT_INFORMATION(Status)          ((((ULONG)(Status)) >> 30) == 1)
#define NT_WARNING(Status)              ((((ULONG)(Status)) >> 30) == 2)
#define NT_ERROR(Status)                ((((ULONG)(Status)) >> 30) == 3)

//
// Limits
//
#define MINCHAR                         0x80
#define MAXCHAR                         0x7f
#define MINSHORT                        0x8000
#define MAXSHORT                        0x7fff
#define MINLONG                         0x80000000
#define MAXLONG                         0x7fffffff
#define MAXUCHAR                        0xff
#define MAXUSHORT                       0xffff
#define MAXULONG                        0xffffffff

//
// CSR Macros
//
#define CSR_MAKE_OPCODE(s,m)            ((s) << 16) | (m)
#define CSR_API_ID_FROM_OPCODE(n)       ((ULONG)((USHORT)(n)))
#define CSR_SERVER_ID_FROM_OPCODE(n)    (ULONG)((n) >> 16)

//
// Basic Types that aren't defined in User-Mode Headers
//
typedef CONST int CINT;
typedef CONST char *PCSZ;
typedef ULONG CLONG;
typedef short CSHORT;
typedef CSHORT *PCSHORT;
typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;
typedef LONG KPRIORITY;
typedef LONG NTSTATUS, *PNTSTATUS;

//
// Basic NT Types
//
#if !defined(_NTSECAPI_H) && !defined(_SUBAUTH_H) && !defined(_NTSECAPI_)

//
// HACKHACK: 2026.2.21 zhuWin  
// Compatibility types for MSVC 4.2 and Windows NT 4.0 DDK
// Doubao-Seed-Code and Gemini 3.1 Pro (Low)
//
#ifndef SIZE_T
#define SIZE_T ULONG
#endif

#ifndef PSIZE_T
typedef SIZE_T *PSIZE_T;
#endif

#ifndef ULONGLONG
typedef unsigned __int64 ULONGLONG;
typedef signed __int64 LONGLONG;
typedef ULONGLONG *PULONGLONG;
typedef LONGLONG *PLONGLONG;
#endif

#ifndef ULONG64
typedef unsigned __int64 ULONG64;
typedef signed __int64 INT64;
typedef ULONG64 *PULONG64;
typedef INT64 *PINT64;
#endif

#ifndef KAFFINITY
typedef ULONG KAFFINITY;
typedef KAFFINITY *PKAFFINITY;
#endif

#ifndef FILE_SEGMENT_ELEMENT_DEFINED
#define FILE_SEGMENT_ELEMENT_DEFINED
typedef union _FILE_SEGMENT_ELEMENT {
    PVOID Buffer;
    ULONGLONG Alignment;
} FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;
#endif

//
// API Calling Convention Macros
//
#ifndef NTSYSCALLAPI
#define NTSYSCALLAPI
#endif

#ifndef NTAPI
#define NTAPI __stdcall
#endif

#ifndef FORCEINLINE
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __inline
#endif
#endif

#ifndef _PVECTORED_EXCEPTION_HANDLER_DEFINED
#define _PVECTORED_EXCEPTION_HANDLER_DEFINED
typedef LONG (NTAPI *PVECTORED_EXCEPTION_HANDLER)(
    struct _EXCEPTION_POINTERS *ExceptionInfo
);
#endif

//
// OSVERSIONINFO Structures
//
#ifndef OSVERSIONINFOW
typedef struct _OSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR szCSDVersion[128];
} OSVERSIONINFOW, *LPOSVERSIONINFOW;
#endif

#ifndef OSVERSIONINFOEXW
typedef struct _OSVERSIONINFOEXW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR szCSDVersion[128];
    WORD wServicePackMajor;
    WORD wServicePackMinor;
    WORD wSuiteMask;
    BYTE wProductType;
    BYTE wReserved;
} OSVERSIONINFOEXW, *LPOSVERSIONINFOEXW;
#endif

//
// RTL Version Info redefinitions
//
typedef OSVERSIONINFOW RTL_OSVERSIONINFOW;
typedef LPOSVERSIONINFOW PRTL_OSVERSIONINFOW;
typedef OSVERSIONINFOEXW RTL_OSVERSIONINFOEXW;
typedef LPOSVERSIONINFOEXW PRTL_OSVERSIONINFOEXW;

//
// GUID Definitions
//
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    DWORD Data1;
    WORD Data2;
    WORD Data3;
    BYTE Data4[8];
} GUID;
#endif

#ifndef _LPGUID_DEFINED
#define _LPGUID_DEFINED
typedef GUID *LPGUID;
#endif

#ifndef IID_DEFINED
#define IID_DEFINED
typedef GUID IID;
typedef GUID *LPIID;
#endif

#ifndef PNP_VETO_TYPE
typedef enum _PNP_VETO_TYPE
{
    PNP_VetoTypeUnknown,
    PNP_VetoTypeDevice,
    PNP_VetoTypeDriver,
    PNP_VetoTypeInvalidDeviceRequest,
    PNP_VetoTypeInsufficientPower,
    PNP_VetoTypeUnsafeRemoval,
    PNP_VetoTypeLegacyDevice,
    PNP_VetoTypePortInUse,
    PNP_VetoTypeHubPort,
    PNP_VetoTypeMaxDefined
} PNP_VETO_TYPE, *PPNP_VETO_TYPE;
#endif

// 
// END HACKHACK
// 

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING, *PSTRING;

typedef struct _CSTRING
{
    USHORT Length;
    USHORT MaximumLength;
    CONST CHAR *Buffer;
} CSTRING, *PCSTRING;

#endif

typedef struct _OBJECT_ATTRIBUTES
{
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

//
// ClientID Structure
//
typedef struct _CLIENT_ID
{
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef const UNICODE_STRING* PCUNICODE_STRING;
typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;
typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;
typedef CONST STRING* PCOEM_STRING;
typedef STRING CANSI_STRING;
typedef PSTRING PCANSI_STRING;

#ifndef ANSI_NULL
#define ANSI_NULL ((CHAR)0)
#endif

#ifndef _POWER_ACTION_DEFINED
#define _POWER_ACTION_DEFINED
typedef enum _POWER_ACTION {
    PowerActionNone,
    PowerActionReserved,
    PowerActionSleep,
    PowerActionHibernate,
    PowerActionShutdown,
    PowerActionShutdownReset,
    PowerActionShutdownOff,
    PowerActionWarmEject
} POWER_ACTION, *PPOWER_ACTION;
#endif

#ifndef _SYSTEM_POWER_STATE_DEFINED
#define _SYSTEM_POWER_STATE_DEFINED
typedef enum _SYSTEM_POWER_STATE {
    PowerSystemUnspecified = 0,
    PowerSystemWorking = 1,
    PowerSystemSleeping1 = 2,
    PowerSystemSleeping2 = 3,
    PowerSystemSleeping3 = 4,
    PowerSystemHibernate = 5,
    PowerSystemShutdown = 6,
    PowerSystemMaximum = 7
} SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE;
#endif

#ifndef _POWER_INFORMATION_LEVEL_DEFINED
#define _POWER_INFORMATION_LEVEL_DEFINED
typedef ULONG POWER_INFORMATION_LEVEL;
#endif

#ifndef _JOB_SET_ARRAY_DEFINED
#define _JOB_SET_ARRAY_DEFINED
typedef struct _JOB_SET_ARRAY {
    HANDLE JobHandle;
    ULONG MemberLevel;
    ULONG Flags;
} JOB_SET_ARRAY, *PJOB_SET_ARRAY;
#endif

#ifndef _JOBOBJECTINFOCLASS_DEFINED
#define _JOBOBJECTINFOCLASS_DEFINED
typedef enum _JOBOBJECTINFOCLASS {
    JobObjectBasicAccountingInformation = 1,
    JobObjectBasicLimitInformation,
    JobObjectBasicProcessIdList,
    JobObjectBasicUIRestrictions,
    JobObjectSecurityLimitInformation,
    JobObjectEndOfJobTimeInformation,
    JobObjectAssociateCompletionPortInformation,
    JobObjectBasicAndIoAccountingInformation,
    JobObjectExtendedLimitInformation,
    JobObjectJobSetInformation,
    MaxJobObjectInfoClass
} JOBOBJECTINFOCLASS;
#endif

#ifndef _PISECURITY_DESCRIPTOR_RELATIVE_DEFINED
#define _PISECURITY_DESCRIPTOR_RELATIVE_DEFINED
typedef PVOID PISECURITY_DESCRIPTOR_RELATIVE;
typedef PVOID PSECURITY_DESCRIPTOR_RELATIVE;
#endif

#ifndef _WORKERCALLBACKFUNC_DEFINED
#define _WORKERCALLBACKFUNC_DEFINED
typedef VOID (NTAPI *WORKERCALLBACKFUNC)(PVOID);
#endif

#ifndef _WAITORTIMERCALLBACKFUNC_DEFINED
#define _WAITORTIMERCALLBACKFUNC_DEFINED
typedef VOID (NTAPI *WAITORTIMERCALLBACKFUNC)(PVOID, BOOLEAN);
#endif

#ifndef _SLIST_ENTRY_DEFINED
#define _SLIST_ENTRY_DEFINED
typedef struct _SLIST_ENTRY {
    struct _SLIST_ENTRY *Next;
} SLIST_ENTRY, *PSLIST_ENTRY;

typedef union _SLIST_HEADER {
    ULONGLONG Alignment;
    struct {
        SLIST_ENTRY Next;
        WORD   Depth;
        WORD   Sequence;
    } DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;
#endif

#ifndef _OBJECT_TYPE_LIST_DEFINED
#define _OBJECT_TYPE_LIST_DEFINED
typedef struct _OBJECT_TYPE_LIST {
    WORD Level;
    WORD Sbz;
    GUID *ObjectType;
} OBJECT_TYPE_LIST, *POBJECT_TYPE_LIST;
#endif

#endif
