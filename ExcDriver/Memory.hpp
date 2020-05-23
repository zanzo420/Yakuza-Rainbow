#pragma once
#include <cstddef>
#include <intrin.h>
#include <ntifs.h>
#include <ntdef.h>

extern "C" __declspec(dllimport)
NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS FromProcess,
	PVOID FromAddress,
	PEPROCESS ToProcess,
	PVOID ToAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T NumberOfBytesCopied
);

extern "C" __declspec(dllimport)
PVOID NTAPI PsGetProcessSectionBaseAddress
(
	PEPROCESS Process	
);

typedef struct _MEMORY_STRUCT
{
	UINT_PTR	process_id;
	PVOID		address;
	SIZE_T		size;
	SIZE_T		size_copied;
	PVOID		buffer;
} MEMORY_STRUCT, * PMEMORY_STRUCT;

namespace Memory
{
	// Allows writing to read-only memory
	// by changing CR0 (control register zero)
	static void WriteProtectOff()
	{
		auto cr0 = __readcr0();
		cr0 &= 0xfffffffffffeffff;
		__writecr0(cr0);
		_disable();
	}

	static void WriteProtectOn()
	{
		auto cr0 = __readcr0();
		cr0 |= 0x10000;
		_enable();
		__writecr0(cr0);
	}
	namespace InternalMemory
	{
		static void Read(void* lpAddress, void* lpBuffer, size_t size, ULONG pid)
		{
			if (!lpAddress || !lpBuffer || !size)
				return;
			if (!pid)
				memcpy(lpBuffer, lpAddress, size);
			else
				__try
			{
				PEPROCESS sourceProcess = nullptr;
				PEPROCESS targetProcess = PsGetCurrentProcess();

				if (!NT_SUCCESS(PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(pid), &sourceProcess))) return;

				SIZE_T dwSizeCopied;
				MmCopyVirtualMemory
				(
					sourceProcess,
					lpAddress,
					targetProcess,
					lpBuffer,
					size,
					KernelMode,
					&dwSizeCopied
					);
			}
			__except (SYSTEM_SERVICE_EXCEPTION | EXCEPTION_EXECUTE_FAULT) {}
		}

		static void Write(void* lpAddress, void* lpBuffer, size_t size, ULONG pid)
		{
			if (!lpAddress || !lpBuffer || !size)
				return;
			if (!pid)
				memcpy(lpAddress, lpBuffer, size);
			else
				__try
			{
				PEPROCESS sourceProcess = nullptr;
				PEPROCESS targetProcess = PsGetCurrentProcess();
				if (!NT_SUCCESS(PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(pid), &sourceProcess))) 
					return;
				SIZE_T dwSizeCopied;
				MmCopyVirtualMemory
				(
					targetProcess,
					lpBuffer,
					sourceProcess,
					lpAddress,
					size,
					KernelMode,
					&dwSizeCopied
					);
			}
			__except (SYSTEM_SERVICE_EXCEPTION | EXCEPTION_EXECUTE_FAULT) {}
		}

	}
	
	namespace Kernel 
	{
		template<class T>
		T Read(void* lpAddress)
		{
			if (!lpAddress) 
				return {};
			T buffer;
			InternalMemory::Read(lpAddress, &buffer, sizeof(T), NULL);
			return buffer;
		}

		template<class T>
		void Write(void* lpAddress, T& data, bool protection = false)
		{
			if (!lpAddress)
				return;
			if (protection)
			{
				WriteProtectOff();
				InternalMemory::Write(lpAddress, &data, sizeof(T), NULL);
				WriteProtectOn();
			}
			else 
				InternalMemory::Write(lpAddress, &data, sizeof(T), NULL);
		}
	}

	namespace UserMode 
	{
		template<class T>
		T Read(ULONG pid, void* lpAddress)
		{
			if (!pid || !lpAddress)
				return {};
			T buffer;
			InternalMemory::Read(lpAddress, &buffer, sizeof(T), pid);
			return buffer;
		}

		template<class T>
		void Write(ULONG pid, void* lpAddress, const T& data)
		{
			if (!lpAddress || !data) 
				return;
			InternalMemory::Write(lpAddress, &data, sizeof(T), pid);
		}

		static PEPROCESS ObtainProcess(HANDLE process_id)
		{
			PEPROCESS process;
			if (!NT_SUCCESS(PsLookupProcessByProcessId(process_id, &process)))
				return NULL;
			return process;
		}

		static PVOID ObtainProcessBase(HANDLE process_id)
		{
			PEPROCESS process = ObtainProcess(process_id);
			if (!process)
				return NULL;
			return PsGetProcessSectionBaseAddress(process);
		}
	}
}