#include "Memory.hpp"
#include "Hook.h"
#include "module.h"
#include <wdm.h>
#define Log(...) DbgPrintEx( DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[Exc] " __VA_ARGS__ )
#include <utility>
#include "watermark/mem_scan.hpp"
#include "watermark/mem_iter.hpp"
#include "watermark/mem_util.hpp"

namespace Driver
{
	static Hook dxgkrnlHook;
	static PVOID Base;
	static ULONG PoolTag;
	static PVOID HookedFunctionAddr;
}

extern "C" _declspec(dllexport) UINT_PTR __fastcall Handler(UINT_PTR CheckCode, UINT_PTR uCode, UINT_PTR Param) 
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

		Log(" Driver Memory Freed");

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
	PCWSTR DriverName)
{
	//// Get Hook Address
	////L"dxgkrnl.sys", "NtDxgkGetProcessList" 1803
	////NtTokenManagerConfirmOutstandingAnalogToken universal?
	////NtDxgkGetProcessList Cencored
	Driver::HookedFunctionAddr = GetSystemModuleExport(L"dxgkrnl.sys", "NtDxgkGetProcessList");

	//Log(" HA: 0x%p\n", Driver::HookedFunctionAddr);

	//// Clearing PiDDB before we hook
	ClearPiDDB(DriverName, TimeDateStamp, PiDDBLockPtr, PiDDBCacheTablePtr);
	if (ClearUnloadedDrivers((uintptr_t)GetNtoskrnlBase())) {
		Log(" CUDF");
	}
	if (CleanUnloadedDrivers()) {
		Log(" CUD");
	}
	//// Init the hook class and Install the hook
	Driver::dxgkrnlHook.HookInit(&Handler, Driver::HookedFunctionAddr);

	/*const auto csrss_process = impl::search_for_process("csrss.exe");
	if (!csrss_process)
		return STATUS_UNSUCCESSFUL;

	impl::unique_attachment csrss_attach(csrss_process);
	const auto win32kfull_info = impl::search_for_module("win32kfull.sys");

	if (!win32kfull_info)
	{
		DbgPrint("[Exc] failed to find win32kfull?");
		return STATUS_UNSUCCESSFUL;
	}

	const auto gpsi_instruction = impl::search_for_signature(win32kfull_info, "\x48\x8b\x0d\x00\x00\x00\x00\x48\x8b\x05\x00\x00\x00\x00\x0f\xba\x30\x0c", "xxx????xxx????xxxx");

	if (!gpsi_instruction)
	{
		DbgPrint("[Exc] failed to find gpsi, signature outdated?");
		return STATUS_UNSUCCESSFUL;
	}

	const auto gpsi = *reinterpret_cast<std::uint64_t*>(impl::resolve_mov(gpsi_instruction));

	if (!gpsi)
		return STATUS_UNSUCCESSFUL;
	*reinterpret_cast<std::uint32_t*>(gpsi + 0x874) = 0;
	DbgPrint("[Exc] No more watermark");*/

	return STATUS_SUCCESS;
}
