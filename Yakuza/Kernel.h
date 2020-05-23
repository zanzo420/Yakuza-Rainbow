#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <memory>
#include <string>

enum CommandCodes {
	ReadPMemory = 1,
	WritePMemory = 2,
	ReadKMemory = 3,
	WriteKMemory = 4,
	GetModuleBase = 5,
	FreeDvr = 6,
};

typedef struct _MEMORY_STRUCT
{
	uint64_t	process_id;
	void* address;
	uint64_t	size;
	uint64_t	size_copied;
	void* buffer;
	uint64_t	struct_value;
} MEMORY_STRUCT, * PMEMORY_STRUCT;

template<typename ... A>
uint64_t call_driver_control(void* control_function, const A ... arguments)
{
	if (!control_function)
		return 0;

	using tFunction = uint64_t(__stdcall*)(A...);
	const auto control = static_cast<tFunction>(control_function);

	return control(arguments ...);
}
inline void* driver_control;
void* KernelFunction();
uint64_t read_kernel(void* control_function, uint64_t address, void* buffer, std::size_t size);

static ULONG64 GetModuleBaseAddr(const char* moduleName, uint32_t ProcessID, CommandCodes code) {
	driver_control = KernelFunction();

	uintptr_t baseADD = call_driver_control(
		driver_control, 0xF00DBABE, code, ProcessID);

	return baseADD;
}

DWORD GetPID(const std::string& name);

inline uint32_t pid;
inline uint64_t base_address;

template<typename T>
T RPM(uintptr_t address)
{
	T buffer = {};
	if (!pid || address > 0x7fffffff0000 || address < 0xffffff)//anti bsod
		return buffer;

	MEMORY_STRUCT memory_struct = { 0 };
	memory_struct.process_id = pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.buffer = &buffer;

	NTSTATUS result
		= (NTSTATUS)(call_driver_control(driver_control, 0xF00DBABE, ReadPMemory, &memory_struct));

	return buffer;
}

template<class T>
T Readclass(uint64_t address)
{
	T buffer = T();

	if (!pid || address > 0x7fffffff0000 || address < 0xffffff)//anti bsod
		return buffer;

	MEMORY_STRUCT memory_struct = { 0 };
	memory_struct.process_id = pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.buffer = &buffer;

	uint64_t result = call_driver_control(driver_control, 0xF00DBABE, ReadPMemory, &memory_struct);

	if (result != 0)
		return T();

	return buffer;
}

template<typename T>
void WPM(uintptr_t address, T buffer)
{
	MEMORY_STRUCT memory_struct = { 0 };
	memory_struct.process_id = pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.buffer = &buffer;

	call_driver_control(driver_control, 0xF00DBABE, WritePMemory, &memory_struct);
}

template<typename T>
inline void ReadArray(uintptr_t address, T* array, size_t len)
{
	if (!pid || address > 0x7fffffff0000 || address < 0xffffff)//anti bsod
		return;

	MEMORY_STRUCT memory_struct = { 0 };
	memory_struct.process_id = pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T) * len;
	memory_struct.buffer = array;

	call_driver_control(driver_control, 0xF00DBABE, ReadPMemory, &memory_struct);
}