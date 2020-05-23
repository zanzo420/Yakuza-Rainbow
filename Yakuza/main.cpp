//#include <vld.h>
#include "Yakuza.h"
#include <Windows.h>
#include <cstdio>
#include "Menu.hpp"
#include "Timer.hpp"

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "user32.lib") 
#pragma comment(lib, "cryptlib.lib")
#pragma comment(lib, "client.lib")

int main()
{
    LI_FN(FreeConsole)();
    enum priority_class 
    {
        realtime = 0x00000100,
        high = 0x00000080,
        above_normal = 0x00008000,
        normal = 0x00000020,
        below_normal = 0x00004000
    };

    SetPriorityClass(GetCurrentProcess(), high);

    std::thread Menu(Cheat.MenuLoop);
    Menu.detach();

    std::thread Features(Cheat.FeatureLoop);
    Features.detach();

    std::thread aim(Cheat.AimThread);
    aim.detach();
 
    for (;; Sleep(10000));
    return 0;
}