#include "Memory.hpp"
#include "Hook.h"
#include "module.h"
#include <wdm.h>


namespace Driver
{
	static Hook dxgkrnlHook;
	static PVOID Base;
	static ULONG PoolTag;
	static PVOID HookedFunctionAddr;
}

extern "C" _declspec(dllexport) UINT_PTR Handler(UINT_PTR CheckCode, UINT_PTR uCode, UINT_PTR Param) 
{
	UNREFERENCED_PARAMETER(CheckCode);
	UNREFERENCED_PARAMETER(uCode);
	UNREFERENCED_PARAMETER(Param);

	if (CheckCode != 0xF00DBABE) 
	{
		//Uninstall
		Driver::dxgkrnlHook.Uninstall();
		//Call Orginal Function
		__int64 returnVal = reinterpret_cast<__int64(*)(__int64 a1, __int64 a2, __int64 a3)>(Driver::HookedFunctionAddr)(CheckCode, uCode, Param);
		//ReInstall Hook
		Driver::dxgkrnlHook.Install();
		//Return Orginal function return.
		return returnVal;
	}

	switch (uCode) 
	{
	case ReadPMemory:
		Memory::InternalMemory::Read
		(
		(void*)reinterpret_cast<PMEMORY_STRUCT>(Param)->address, 
			reinterpret_cast<PMEMORY_STRUCT>(Param)->buffer, 
			reinterpret_cast<PMEMORY_STRUCT>(Param)->size, 
			(ULONG)reinterpret_cast<PMEMORY_STRUCT>(Param)->process_id
		);
		break;
	case WritePMemory:
		Memory::InternalMemory::Write
		(
		(void*)reinterpret_cast<PMEMORY_STRUCT>(Param)->address,
			reinterpret_cast<PMEMORY_STRUCT>(Param)->buffer,
			reinterpret_cast<PMEMORY_STRUCT>(Param)->size,
			(ULONG)reinterpret_cast<PMEMORY_STRUCT>(Param)->process_id
		);
		break;
	case GetModuleBase:
		return reinterpret_cast<UINT_PTR>(Memory::UserMode::ObtainProcessBase(reinterpret_cast<HANDLE>(Param)));
		break;
	case FreeDvr:
		if (!Driver::Base || !Driver::PoolTag)
			return STATUS_ABANDONED;

		Driver::dxgkrnlHook.Uninstall();

		DbgPrint("[+] Driver Memory Freed");

		FreeDriver
		(
			(uintptr_t)Driver::Base,
			Driver::PoolTag,
			_ReturnAddress(),
			&ExFreePoolWithTag
		);
		break;
	default:
		break;
	
	}
	return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry(
	UINT_PTR PoolAddress, 
	ULONG PoolTag, 
	PUCHAR PiDDBLockPtr, 
	PUCHAR PiDDBCacheTablePtr, 
	ULONG TimeDateStamp, 
	PCWSTR DriverName
	)
{
	// Print what Entry is called with
	DbgPrint("[+] PoolAddress 0x%p\n PoolTag %d\n", PoolAddress, PoolTag);
	// Setup to be able to FreeDriver
	//wnd_hjk::PoolTag = PoolTag;
	Driver::PoolTag = PoolTag;
	Driver::Base = (PVOID)PoolAddress;

	// Get Hook Address
	//L"dxgkrnl.sys", "NtDxgkGetProcessList" 1803
	//NtTokenManagerConfirmOutstandingAnalogToken universal?
	//NtDxgkGetProcessList Cencored
	Driver::HookedFunctionAddr = GetSystemModuleExport(L"dxgkrnl.sys", "NtDxgkGetProcessList");

	DbgPrint("[+] HookAddress: 0x%p\n", Driver::HookedFunctionAddr);
	
	DbgPrint("[+] Clearing PiDDB of: %s, Time 0x%p, PiDDBLockPtr: %s, PiDDBCacheTablePtr %s", DriverName, TimeDateStamp, PiDDBLockPtr, PiDDBCacheTablePtr);
	
	// Clearing PiDDB before we hook
	ClearPiDDB(DriverName, TimeDateStamp, PiDDBLockPtr, PiDDBCacheTablePtr);
	ClearUnloadedDrivers((uintptr_t)GetNtoskrnlBase());
	// Init the hook class and Install the hook
	Driver::dxgkrnlHook.HookInit(&Handler, Driver::HookedFunctionAddr);

	return STATUS_SUCCESS;
}