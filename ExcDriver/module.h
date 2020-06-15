#pragma once
#include <ntimage.h>
#include "ExternalFunctions.h"

PVOID GetSystemRoutineAddress(LPCWSTR routine_name)
{
	UNICODE_STRING name;
	RtlInitUnicodeString(&name, routine_name);
	return MmGetSystemRoutineAddress(&name);
}

PVOID GetSystemModuleBase(LPCWSTR module_name)
{


	PVOID module_base = NULL;

	__try {

		PLIST_ENTRY module_list = reinterpret_cast<PLIST_ENTRY>(GetSystemRoutineAddress(L"PsLoadedModuleList"));

		if (!module_list)
			return NULL;

		UNICODE_STRING name;
		RtlInitUnicodeString(&name, module_name);

		//  InLoadOrderlinks.Flink at 0xffff8f8a`0f25f110
		//	-------------------------------------------- -
		//	+ 0x000 InLoadOrderlinks :  [0xffff8f8a`0cee8c90 - 0xffff8f8a`0f25b010]
		//	+ 0x048 FullDllName : _UNICODE_STRING "\SystemRoot\System32\win32kbase.sys"

		for (PLIST_ENTRY link = module_list; link != module_list->Blink; link = link->Flink)
		{
			LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

			//DbgPrint( "driver: %ws\n", entry->FullDllName.Buffer );

			if (RtlEqualUnicodeString(&entry->BaseDllName, &name, TRUE))
			{
				module_base = entry->DllBase;
				break;
			}
		}

	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

		module_base = NULL;
	}

	return module_base;
}

PVOID GetSystemModuleExport(LPCWSTR module_name, LPCSTR routine_name)
{
	PVOID lpModule = GetSystemModuleBase(module_name);

	DbgPrint("> Address of module base: 0x%p\n", lpModule);

	if (!lpModule)
		return NULL;

	return RtlFindExportedRoutineByName(lpModule, routine_name);
}

PVOID GetNtoskrnlBase()
{
	ULONG bytes = 0;
	ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	PRTL_PROCESS_MODULES modules = reinterpret_cast<PRTL_PROCESS_MODULES>(ExAllocatePool(NonPagedPool, bytes));
	RtlSecureZeroMemory(modules, bytes);

	// DbgPrint( "> allocate %lu bytes for modules\n", bytes );

	if (!NT_SUCCESS(ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes)))
	{
		ExFreePool(modules);
		return nullptr;
	}

	// DbgPrint( "> ntoskrnl: %llx\n", modules->Modules[0].ImageBase );
	// DbgPrint( "> ntoskrnl: %s\n", modules->Modules[0].FullPathName );

	ExFreePool(modules);
	return modules->Modules[0].ImageBase;
}

NTSTATUS BBSearchPattern(IN PCUCHAR pattern, IN UCHAR wildcard, IN ULONG_PTR len, IN const VOID* base, IN ULONG_PTR size, OUT PVOID* ppFound, int index = 0)
{
	ASSERT(ppFound != NULL && pattern != NULL && base != NULL);
	if (ppFound == NULL || pattern == NULL || base == NULL)
		return STATUS_ACCESS_DENIED; //STATUS_INVALID_PARAMETER;
	int cIndex = 0;
	for (ULONG_PTR i = 0; i < size - len; i++)
	{
		BOOLEAN found = TRUE;
		for (ULONG_PTR j = 0; j < len; j++)
		{
			if (pattern[j] != wildcard && pattern[j] != ((PCUCHAR)base)[i + j])
			{
				found = FALSE;
				break;
			}
		}

		if (found != FALSE && cIndex++ == index)
		{
			*ppFound = (PUCHAR)base + i;
			return STATUS_SUCCESS;
		}
	}

	return STATUS_NOT_FOUND;
}

NTSTATUS BBScanSection(IN PCCHAR section, IN PCUCHAR pattern, IN UCHAR wildcard, IN ULONG_PTR len, OUT PVOID* ppFound, PVOID base = nullptr)
{
	//ASSERT(ppFound != NULL);
	if (ppFound == NULL)
		return STATUS_ACCESS_DENIED; //STATUS_INVALID_PARAMETER

	if (nullptr == base)
		base = GetNtoskrnlBase();
	if (base == nullptr)
		return STATUS_ACCESS_DENIED; //STATUS_NOT_FOUND;

	PIMAGE_NT_HEADERS64 pHdr = RtlImageNtHeader(base);
	if (!pHdr)
		return STATUS_ACCESS_DENIED; // STATUS_INVALID_IMAGE_FORMAT;

	//PIMAGE_SECTION_HEADER pFirstSection = (PIMAGE_SECTION_HEADER)(pHdr + 1);
	PIMAGE_SECTION_HEADER pFirstSection = (PIMAGE_SECTION_HEADER)((uintptr_t)&pHdr->FileHeader + pHdr->FileHeader.SizeOfOptionalHeader + sizeof(IMAGE_FILE_HEADER));

	for (PIMAGE_SECTION_HEADER pSection = pFirstSection; pSection < pFirstSection + pHdr->FileHeader.NumberOfSections; pSection++)
	{
		//DbgPrint("section: %s\r\n", pSection->Name);
		ANSI_STRING s1, s2;
		RtlInitAnsiString(&s1, section);
		RtlInitAnsiString(&s2, (PCCHAR)pSection->Name);
		if (RtlCompareString(&s1, &s2, TRUE) == 0)
		{
			PVOID ptr = NULL;
			NTSTATUS status = BBSearchPattern(pattern, wildcard, len, (PUCHAR)base + pSection->VirtualAddress, pSection->Misc.VirtualSize, &ptr);
			if (NT_SUCCESS(status)) {
				*(PULONG64)ppFound = (ULONG_PTR)(ptr); //- (PUCHAR)base
				//DbgPrint("found\r\n");
				return status;
			}
			//we continue scanning because there can be multiple sections with the same name.
		}
	}

	return STATUS_ACCESS_DENIED; //STATUS_NOT_FOUND;
}

EXTERN_C PVOID ResolveRelativeAddress(
	_In_ PVOID Instruction,
	_In_ ULONG OffsetOffset,
	_In_ ULONG InstructionSize
	)
{
	ULONG_PTR Instr = (ULONG_PTR)Instruction;
	LONG RipOffset = *(PLONG)(Instr + OffsetOffset);
	PVOID ResolvedAddr = (PVOID)(Instr + InstructionSize + RipOffset);

	return ResolvedAddr;
}

extern "C" bool LocatePiDDB(PERESOURCE * lock, PRTL_AVL_TABLE * table, PUCHAR PiDDBLockPtr_sig, PUCHAR PiDDBCacheTablePtr_sig)
{
	PVOID PiDDBLockPtr = nullptr, PiDDBCacheTablePtr = nullptr;
	if (!NT_SUCCESS(BBScanSection("PAGE", PiDDBLockPtr_sig, 0, sizeof(PiDDBLockPtr_sig) - 1, reinterpret_cast<PVOID*>(&PiDDBLockPtr)))) {
		//DbgPrintEx(0, 0, xorstr_("Unable to find PiDDBLockPtr sig.\n"));
		return false;
	}

	if (!NT_SUCCESS(BBScanSection("PAGE", PiDDBCacheTablePtr_sig, 0, sizeof(PiDDBCacheTablePtr_sig) - 1, reinterpret_cast<PVOID*>(&PiDDBCacheTablePtr)))) {
		//DbgPrintEx(0, 0, xorstr_("Unable to find PiDDBCacheTablePtr sig.\n"));
		return false;
	}

	PiDDBCacheTablePtr = PVOID((uintptr_t)PiDDBCacheTablePtr + 3);

	*lock = (PERESOURCE)(ResolveRelativeAddress(PiDDBLockPtr, 3, 7));
	*table = (PRTL_AVL_TABLE)(ResolveRelativeAddress(PiDDBCacheTablePtr, 3, 7));

	return true;
}

PVOID ClearPiDDB(PCWSTR driverName, ULONG TimeDateStamp, PUCHAR PiDDBLockPtr_sig, PUCHAR PiDDBCacheTablePtr_sig)
{
	// first locate required variables
	PERESOURCE PiDDBLock; PRTL_AVL_TABLE PiDDBCacheTable;
	if (!LocatePiDDB(&PiDDBLock, &PiDDBCacheTable, PiDDBLockPtr_sig, PiDDBCacheTablePtr_sig))
	{
		DbgPrint("[-] PiDDB not found :(");
		return NULL;
	}

	// build a lookup entry
	PiDDBCacheEntry lookupEntry = { };
	UNICODE_STRING DriverName;
	RtlInitUnicodeString(&DriverName, L"phymem64.sys");
	lookupEntry.DriverName = DriverName;
	lookupEntry.TimeDateStamp = TimeDateStamp;

	// acquire the ddb resource lock
	ExAcquireResourceExclusiveLite(PiDDBLock, TRUE);

	// search our entry in the table
	auto pFoundEntry = (PiDDBCacheEntry*)RtlLookupElementGenericTableAvl(PiDDBCacheTable, &lookupEntry);
	if (pFoundEntry == nullptr)
	{
		DbgPrint("[-] Can't find driver in list :(");
		// release the ddb resource lock
		ExReleaseResourceLite(PiDDBLock);

		return NULL;
	}

	// first, unlink from the list
	RemoveEntryList(&pFoundEntry->List);
	// then delete the element from the avl table
	if (!RtlDeleteElementGenericTableAvl(PiDDBCacheTable, pFoundEntry)) DbgPrint("[-] Couldn't remove entry from Avl table");

	// release the ddb resource lock
	ExReleaseResourceLite(PiDDBLock);
	return NULL;
}

BOOLEAN bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;

	return (*szMask) == 0;
}

UINT64 FindPattern(UINT64 dwAddress, UINT64 dwLen, BYTE* bMask, char* szMask)
{
	for (UINT64 i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (UINT64)(dwAddress + i);

	return 0;
}

BOOLEAN CleanUnloadedDrivers()
{
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes)
	{
		DbgPrint("CleanUnloadedDrivers: first NtQuerySystemInformation failed, status: 0x%x", status);
		return FALSE;
	}

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x454E4F45); // 'ENON'

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("CleanUnloadedDrivers: second NtQuerySystemInformation failed, status: 0x%x", status);
		return FALSE;
	}

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	UINT64 ntoskrnlBase = 0, ntoskrnlSize = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		DbgPrint("CleanUnloadedDrivers: path: %s", module[i].FullPathName);

		if (!strcmp((char*)module[i].FullPathName, "\\SystemRoot\\system32\\ntoskrnl.exe"))
		{
			ntoskrnlBase = (UINT64)module[i].ImageBase;
			ntoskrnlSize = (UINT64)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, 0);

	if (ntoskrnlBase <= 0)
	{
		DbgPrint("CleanUnloadedDrivers: ntoskrnlBase equals zero");
		return FALSE;
	}

	// NOTE: 4C 8B ? ? ? ? ? 4C 8B C9 4D 85 ? 74 + 3] + current signature address = MmUnloadedDrivers
	UINT64 mmUnloadedDriversPtr = FindPattern((UINT64)ntoskrnlBase, (UINT64)ntoskrnlSize, (BYTE*)"\x4C\x8B\x00\x00\x00\x00\x00\x4C\x8B\xC9\x4D\x85\x00\x74", "xx?????xxxxx?x");

	if (!mmUnloadedDriversPtr)
	{
		DbgPrint("CleanUnloadedDrivers: mmUnloadedDriversPtr equals zero");
		return FALSE;
	}

	UINT64 mmUnloadedDrivers = (UINT64)((PUCHAR)mmUnloadedDriversPtr + *(PULONG)((PUCHAR)mmUnloadedDriversPtr + 3) + 7);
	UINT64 bufferPtr = *(UINT64*)mmUnloadedDrivers;

	// NOTE: 0x7D0 is the size of the MmUnloadedDrivers array for win 7 and above
	PVOID newBuffer = ExAllocatePoolWithTag(NonPagedPoolNx, 0x7D0, 0x54446D4D);

	if (!newBuffer)
		return FALSE;

	memset(newBuffer, 0, 0x7D0);

	// NOTE: replace MmUnloadedDrivers
	*(UINT64*)mmUnloadedDrivers = (UINT64)newBuffer;

	// NOTE: clean the old buffer
	ExFreePoolWithTag((PVOID)bufferPtr, 0x54446D4D); // 'MmDT'

	return TRUE;
}
