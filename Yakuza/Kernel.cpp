#include "Kernel.h"
#include <string>

void* KernelFunction()
{
	LoadLibrary("user32.dll");
	HMODULE hModule = LoadLibrary("win32u.dll");

	if (!hModule)
		return nullptr;

	return reinterpret_cast<void*>(GetProcAddress(hModule, "NtDxgkGetProcessList"));
}

DWORD GetPID(const std::string& name)
{
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            if (std::string(process.szExeFile) == name)
            {
                pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }
    return pid;
}