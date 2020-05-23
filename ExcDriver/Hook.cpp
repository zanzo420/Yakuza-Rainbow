#include "Hook.h"

//Checks for NULLS then copies original bytes and installs the hook.
void Hook::HookInit(void* HookAddress, void* HookedFunction)
{
	//Init the Class
	this->Function = HookedFunction;
	this->Address = HookAddress;
	
	if (!HookedFunction || !HookAddress) 
	{ 
		DbgPrint("[-] NULL hook, HookedFunction: 0x%p, Handler: 0x%p\n", HookedFunction, HookAddress); 
		return; 
	}
	//Copy the OG Bytes
	memcpy(this->OriginalShellCode, HookedFunction, sizeof(OriginalShellCode));
	this->Install();
}

void Hook::Install()
{
	if (!this->Function || !this->Address) return;
	//Copy the Address of our function into the shell code thats going to be written to the address of the original function
	memcpy(JumpCode + 0x3, &Address, sizeof(Address));
	Memory::Kernel::Write<decltype(JumpCode)>(Function, JumpCode, true);
	DbgPrint("[+] Installed Hook at address: 0x%p\n", Function);
}

void Hook::Uninstall()
{
	if (!this->Function || !this->Address) return;
	Memory::Kernel::Write<decltype(OriginalShellCode)>(Function, OriginalShellCode, true);
	DbgPrint("[?] Uninstalled Hook at address: 0x%p\n", Function);
}


