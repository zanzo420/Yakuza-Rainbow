#pragma once
#include <windows.h>
#include <iostream>
#include <string_view>
#include <vector>
#include <thread>
#include <atomic>

#include "../util/util.hpp"
#include "../physmeme/physmeme.hpp"
#include "../util/hook.hpp"

namespace physmeme
{
	//
	// offset of function into a physical page
	// used for comparing bytes when searching
	//
	inline std::uint16_t nt_page_offset{};

	//
	// rva of nt function we are going to hook
	//
	inline std::uint32_t nt_rva{};

	//
	// base address of ntoskrnl (inside of this process)
	//
	inline const std::uint8_t* ntoskrnl_buffer{};

	//
	// mapping of a syscalls physical memory (for installing hooks)
	//
	inline std::atomic<void*> psyscall_func{};

	//
	// you can edit this how you choose, im hooking NtShutdownSystem.
	//
	inline const std::pair<std::string_view, std::string_view> syscall_hook = { "NtOpenEnlistment", "ntdll.dll" };

	class kernel_ctx
	{
	public:
		//
		// default constructor
		//
		kernel_ctx();

		//
		// allocate kernel pool of desired size and type
		//
		void* allocate_pool(std::size_t size, POOL_TYPE pool_type = NonPagedPool);

		//
		// allocate kernel pool of size, pool tag, and type
		//
		void* allocate_pool(std::size_t size, ULONG pool_tag = 'LOOB', POOL_TYPE pool_type = NonPagedPool);

		//
		// read kernel memory with RtlCopyMemory
		//
		void read_kernel(void* addr, void* buffer, std::size_t size);

		//
		// write kernel memory with RtlCopyMemory
		//
		void write_kernel(void* addr, void* buffer, std::size_t size);

		//
		// zero kernel memory using RtlZeroMemory 
		//
		void zero_kernel_memory(void* addr, std::size_t size);

		template <class T>
		T read_kernel(void* addr)
		{
			if (!addr)
				return  {};
			T buffer;
			read_kernel(addr, &buffer, sizeof(T));
			return buffer;
		}

		template <class T>
		void write_kernel(void* addr, const T& data)
		{
			if (!addr)
				return {};
			write_kernel(addr, &data, sizeof(T));
		}

		//
		// use this to call any function in the kernel
		//
		template <class T, class ... Ts>
		std::invoke_result_t<T, Ts...> syscall(void* addr, Ts ... args)
		{
			static const auto proc = 
				GetProcAddress(
					GetModuleHandleA("ntdll.dll"),
					syscall_hook.first.data()
				);

			if (!proc || !psyscall_func || !addr)
				return {};

			hook::make_hook(psyscall_func, addr);
			auto result = reinterpret_cast<T>(proc)(args ...);
			hook::remove(psyscall_func);
			return result;
		}

	private:

		//
		// find and map the physical page of a syscall into this process
		//
		void map_syscall(std::uintptr_t begin, std::uintptr_t end) const;
	};
}