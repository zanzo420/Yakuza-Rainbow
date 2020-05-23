#include "../kdmod/kdmapper.hpp"


void mapspoof()
{
	if (intel_driver::IsRunning())
	{ //Already Loaded
		std::cout << "[-] Please Restart Your Computer and Try Again" << std::endl;
		Sleep(5000);
		std::exit(-1);
	}

	HANDLE iqvw64e_device_handle = intel_driver::Load();

	if (!iqvw64e_device_handle || iqvw64e_device_handle == INVALID_HANDLE_VALUE)
	{
		//[-] Failed to load driver iqvw64e.sys
		std::cout << "[-] Failed to load if problem persists contact admin" << std::endl;
		Sleep(5000);
		std::exit(-1);
	}

	if (!kdmapper::MapDriver(iqvw64e_device_handle, 1))
	{
		//std::cout << "[-] Failed to map " << driver_path << std::endl;
		std::cout << "[-] Failed to Initialize if problem persists contact admin" << std::endl;
		Sleep(5000);
		std::exit(-1);
	}

	intel_driver::Unload(iqvw64e_device_handle);
}
