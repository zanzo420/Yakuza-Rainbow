#pragma once
#include <windows.h>
#include <mutex>
#include <cstdint>
#include <map>

#include "physmem64.h"
#include "pmdll64.h"
#include "../util/util.hpp"

namespace physmeme
{
	constexpr const char* driver_path = "C:\\Windows\\phymem64.sys";
	constexpr const char* dll_path = "C:\\Windows\\pmdll64.dll";

	//
	// please code this function depending on your method of physical read/write.
	//
	static std::uintptr_t map_phys(
		std::uintptr_t addr,
		std::size_t size
	)
	{
		//--- ensure the validity of the address we are going to try and map
		if (!util::is_valid(addr))
			return NULL;

		static const auto map_phys_ptr =
			reinterpret_cast<__int64(__fastcall*)(__int64, unsigned)>(
				GetProcAddress(LoadLibrary(dll_path), "MapPhyMem"));
		return map_phys_ptr ? map_phys_ptr(addr, size) : false;
	}

	//
	// please code this function depending on your method of physical read/write.
	//
	static bool unmap_phys(
		std::uintptr_t addr,
		std::size_t size
	)
	{
		static const auto unmap_phys_ptr =
			reinterpret_cast<__int64(*)(__int64, unsigned)>(
				GetProcAddress(LoadLibrary(dll_path), "UnmapPhyMem"));
		return unmap_phys_ptr ? unmap_phys_ptr(addr, size) : false;
	}

	//
	// please code this function depending on your method of physical read/write.
	//
	static HANDLE load_drv()
	{
		std::ofstream driver(driver_path, std::ios_base::out | std::ios_base::binary);
		driver.write((char*)driverdata, sizeof(driverdata));
		driver.close();
		std::ofstream dll(dll_path, std::ios_base::out | std::ios_base::binary);
		dll.write((char*)dlldata, sizeof(dlldata));
		dll.close();

		static const auto load_driver_ptr =
			reinterpret_cast<__int64(*)()>(
				GetProcAddress(LoadLibrary(dll_path), "LoadPhyMemDriver"));

		if (load_driver_ptr)
			load_driver_ptr();

		//--- i dont ever use this handle, its just an example of what you should do.
		return CreateFileA("\\\\.\\PhyMem", 0xC0000000, 3u, 0i64, 3u, 0x80u, 0i64);
	}

	//
	// please code this function depending on your method of physical read/write.
	//
	static bool unload_drv()
	{


		static const auto unload_driver_ptr =
			reinterpret_cast<__int64(*)()>(
				GetProcAddress(LoadLibrary(dll_path), "UnloadPhyMemDriver"));
		std::remove(driver_path);
		std::remove(dll_path);
		return unload_driver_ptr ? unload_driver_ptr() : false;
	}

	static HANDLE drv_handle = load_drv();
}