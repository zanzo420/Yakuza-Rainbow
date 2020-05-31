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

#define version 208
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

void update()
{
    //Check Version
    Socket update_socket = Socket("auth.yakuza.dev");
    std::string data = "", response = "", filename = "", cheat = "yakuza-";
    update_socket.http_get_raw("version", data, response);
    if (std::stoi(response) <= version)
        return;

    char PathToSelf[256];
    GetModuleFileNameA(NULL, PathToSelf, sizeof(PathToSelf));
    //Generate Random EXE name
    //TO DO add polymorphic engine to generate randomized files too
    std::generate_n(std::back_inserter(filename), 16, []()
        {
            thread_local std::mt19937_64 mersenne_generator(std::random_device{ }());
            const std::uniform_int_distribution<> distribution(97, 122);
            return static_cast<std::uint8_t>(distribution(mersenne_generator));
        });
    //Download latest
    filename += ".exe";
    filename.insert(0, "yakuza-");
    std::string download_path = PathToSelf;
    size_t pos = download_path.find(cheat);
    if (pos)
        download_path.replace(pos, cheat.length() + 16, filename);
    else
        return;

    URLDownloadToFile(NULL, _T("http://auth.yakuza.dev/yakuza.exe"), _T(download_path.c_str()), 0, NULL);
    //TODO use sockets

    //////////////////////////////////
    //data = "exe";
    //response = ""; //.clear() might not be safe
    //update_socket.http_get_raw("yakuza.exe", data, response);
    ////Dump it to disk
    //std::ofstream update(filename + ".exe", std::ofstream::app | std::ofstream::binary | std::ofstream::out);
    //update.write((const char*)response.c_str(), sizeof(response.c_str()));
    //////////////////////////////////

    //Setup file name
    std::string stemp = std::string(download_path.begin(), download_path.end());
    LPCSTR filenameW = stemp.c_str();
    //Execute filename with no pa rams
    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;
    CreateProcess(filenameW, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo);
    //Delete our own process
    DelMe();
    quick_exit(0);
}

void main()
{
    LI_FN(FreeConsole)(); // Make sure we don't show the console 
    
    if (GetPID("RainbowSix.exe") && !Cheat.LoggedIn()) // Check if the game is running and exit if it is.
    {
        //quick_exit(EXIT_FAILURE);
    }

    SetPriorityClass(GetCurrentProcess(), high); //Lets us have more processor access and gives the TLB access rights to make us run faster

    update(); // Checks for update

    //Start Cheat thread
    std::thread Menu(Cheat.MenuLoop);        Menu.detach();
    std::thread Features(Cheat.FeatureLoop); Features.detach();
    std::thread aim(Cheat.AimThread);        aim.detach();

    //Close if siege is open
    for (;; Sleep(5000000))
    {
       /*if (GetPID("RainbowSix.exe") && !Cheat.LoggedIn())
       {
            quick_exit(EXIT_FAILURE);
       }*/
    }
}