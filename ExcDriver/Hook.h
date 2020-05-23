#pragma once
#include <cstddef>
#include <ntifs.h>
#include <ntstrsafe.h>
//#include <ntdef.h>
#include <WinDef.h>
#include <wdm.h>
#include <intrin.h>
#include "Memory.hpp"

class Hook
{
public:
	
	Hook() 
	{
		Address = 0x0;
		Function = 0x0;
	};

	void HookInit(void* HookAddress, void* HookedFunction);
	void Install();
	void Uninstall();

private:
	void* Address;
	void* Function;
	BYTE OriginalShellCode[13];
	BYTE JumpCode[13]{ 0x90, 0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0 };
};

