//#include <vld.h>
#include "Server/include/Socket.hpp"
#include "Yakuza.h"
#include "Timer.hpp"
#include <cstdio>
#include <thread>
#include <fstream>
#include <random>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "user32.lib") 
#pragma comment(lib, "cryptlib.lib")

#define version 210
#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")
#define DEV_MODE

enum priority_class
{
    realtime = 0x00000100,
    high = 0x00000080,
    above_normal = 0x00008000,
    normal = 0x00000020,
    below_normal = 0x00004000
};


void DelMe()
{
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    if (GetPID("RainbowSix.exe") && !Cheat.LoggedIn()) // Check if the game is running and exit if it is.
    {
       quick_exit(EXIT_FAILURE);
    }

    SetPriorityClass(GetCurrentProcess(), high); //Lets us have more processor access and gives the TLB access rights to make us run faster

    //Start Cheat thread
    std::thread Menu(Cheat.MenuLoop);        Menu.detach();
    std::thread Features(Cheat.FeatureLoop); Features.detach();
    std::thread aim(Cheat.AimThread);        aim.detach();

    //Close if siege is open
    for (;; Sleep(5000000))
    {
       if (GetPID("RainbowSix.exe") && !Cheat.LoggedIn())
       {
            quick_exit(EXIT_FAILURE);
       }
    }
}