/// Windows socket
#include "Server/include/Licenser.hpp"
// So we include it before Windows.h gets linked internally

#include "../physmeme/map_driver.hpp"
#include "md5.h"
#include "Timer.hpp"
#include "Yakuza.h"
#include "Menu.hpp"
#include "FUNC/callfuncs.h"
#include "map/ExcDrv.h"

#pragma comment(lib, "physmeme-lib.lib")


// Sets the Window Handle that
// Our menu uses to place on r6s
void Yakuza::SetGameWindow()
{
	Menu::Variables::PrevWindow = NULL;

	Menu::Variables::PrevWindow = LI_FN(GetWindow)(FindWindowA(0, ("Rainbow Six")), GW_HWNDPREV);
	
}

// Checks if the Game Window is found
// if not calls SetGameWindow then opens Menu
void Yakuza::MenuLoop()
{
	using namespace std::chrono_literals;
	Timer t{  };
	static bool init = true;
	for (;;) {	
		if (t.hasElapsed(10ms) || !Menu::Variables::init)
		{
			SetGameWindow();
			t.reset();
		}
		
		if (!Menu::Variables::loggedin)
		{
			Menu::Variables::DESKTOP = GetForegroundWindow();
			Menu::Variables::PrevWindow = GetWindow(Menu::Variables::DESKTOP, GW_HWNDPREV);
		}

		Menu::IntializeMenu();
	}
}

bool Yakuza::Login(char* Username, char* license)
{
	std::string server_url = "auth.yakuza.dev";
	std::string rsa_public_key = "MIIEIjANBgkqhkiG9w0BAQEFAAOCBA8AMIIECgKCBAEAyLrzZzFZjuQ+E1VAHqmqBwxZe1W70iD4UxPP3mXMdnW5JjzmDwQF0z1AH0Kl8+YRHU0yew7HexWdQjSnScypsrXTS5w7WIGPnk9E6DpS6WeIekFMXT5Qxn+l9UVD3e0ZVrVeBc9ZXrNsI7JeZLc3yFB1WjgtLjqGG2a6XbXMJJVCccd/yk2uLDlmEDHdtGQRdfNEjy8iydm1W4mU0f6d+4rrgGNexAThXdEnpjHVhA1V/G3nP8tTdW61fPLRCPQo91+99TUdafuzfRj0hgz/HCAcIO3OOzb6rkQ27598SlrAQ4H8Zx90nNDaGrDOwooTPnkpQSpCqoTX74rP78GbGnjIj/IgVeQ5WPmt/D8Ok6UaLuz0dHdqpLCi/XKk9dCYxwni9xeuC50f+l6/kbjrUAgz4uiY0+uc1TTdCnCMI3DmsjV+LakWCkTu/hPcNtrJ7ayGqcLSP6QkTrtjfLfxps7dz49cF/ibCXq0RHaCsz97pXStdQfRdErpYMjkUa4tvL0CWAFPVmGv6gN375XfDvcFLZ8KzViTCH8bH3LuIol3/j+wewmFAXQ0Q3Tr6jGVEJBVTwYCnNwkEcoXUi45oqF3wt5xw3HfYw+M6yh5sa9SzAhnGvwmCaRXcSpNdaM4SQE/3/f5qmIgOO0GEhB7tyM5RruVGmICLK2r787KBv3JjLnbLQmCe03igIDGP3V5O/Rht9oDUu62ViC+xYIjcqM9W5PHhB6gxTPQAWj81svbHpKu8r0f3XDAIoROwLHfz1X5Un2b84vSAs7OwzHMSY2xua5T7xXGDL1wrq+xYRB8yeMq05JHMwvwxZNyWmisRlX0X20sQL4xv0quD/Shs10aOSPnmiM3lieEjdxVoGeMZ3I/51lwCw0uTKFlu65BHz/kVPqVj4ZcqyUjkp8pSJh+nFamFBtKVEiVK4DYvfHWpAqOPKoNvq5XGBsudPYh2z7+2WXlfV8DL29/HeCm15UomhslOTEE2D0y4oXxco3xk51Fbfa/HTw9B7do630dVarzGxfDer/ElGZm2CRpD92uzrcmPI4jwV4uDb92yJ3K73BOOeGAa9S2UxRel5lrg4JLIeed8/R30i7Es6X1kJ307KvalkyA23LOQgzZTPouOr16KGN3wO7e3cw5QVZbI0B1osfZvYOhCJjszKSPdG4v0w8cuV5O28qYsY594YzBRWLnYTggMH3ginYpipNBVwPGtcOiKKNmw/sVVXj5NLF+NbIqRICxcoHB4M5+kcB8zQzPe17l7x8WiyFG0F85Mciz+y7qzqvEJkCOTlQouNvWrAN1RN00mILV+sHBCFnIQorUWEobbCYFJDeDfNHTuXCzL8LLPrATX1pIJ2iqVQIDAQAB";
	if (server_url.empty() || rsa_public_key.empty()) {
		return false;
	}
	auto licenser = Licenser(server_url);
	licenser.load_rsa_public_key(rsa_public_key);

	if (licenser.user_login(Username, license)) 
	{
		Menu::Variables::loggedin = true;
		if (!options::dontMap)
		{
			std::vector<uint8_t> driver_image(sizeof(ExcDrv));
			memcpy(driver_image.data(), ExcDrv, sizeof(ExcDrv));
			physmeme::load_drv();
			physmeme::map_driver(driver_image);
		}
		return true;
	}
	
	return false;
}

bool Yakuza::Register(char* Username, char* license)
{
	std::string server_url = "auth.yakuza.dev";
	std::string rsa_public_key = "MIIEIjANBgkqhkiG9w0BAQEFAAOCBA8AMIIECgKCBAEAyLrzZzFZjuQ+E1VAHqmqBwxZe1W70iD4UxPP3mXMdnW5JjzmDwQF0z1AH0Kl8+YRHU0yew7HexWdQjSnScypsrXTS5w7WIGPnk9E6DpS6WeIekFMXT5Qxn+l9UVD3e0ZVrVeBc9ZXrNsI7JeZLc3yFB1WjgtLjqGG2a6XbXMJJVCccd/yk2uLDlmEDHdtGQRdfNEjy8iydm1W4mU0f6d+4rrgGNexAThXdEnpjHVhA1V/G3nP8tTdW61fPLRCPQo91+99TUdafuzfRj0hgz/HCAcIO3OOzb6rkQ27598SlrAQ4H8Zx90nNDaGrDOwooTPnkpQSpCqoTX74rP78GbGnjIj/IgVeQ5WPmt/D8Ok6UaLuz0dHdqpLCi/XKk9dCYxwni9xeuC50f+l6/kbjrUAgz4uiY0+uc1TTdCnCMI3DmsjV+LakWCkTu/hPcNtrJ7ayGqcLSP6QkTrtjfLfxps7dz49cF/ibCXq0RHaCsz97pXStdQfRdErpYMjkUa4tvL0CWAFPVmGv6gN375XfDvcFLZ8KzViTCH8bH3LuIol3/j+wewmFAXQ0Q3Tr6jGVEJBVTwYCnNwkEcoXUi45oqF3wt5xw3HfYw+M6yh5sa9SzAhnGvwmCaRXcSpNdaM4SQE/3/f5qmIgOO0GEhB7tyM5RruVGmICLK2r787KBv3JjLnbLQmCe03igIDGP3V5O/Rht9oDUu62ViC+xYIjcqM9W5PHhB6gxTPQAWj81svbHpKu8r0f3XDAIoROwLHfz1X5Un2b84vSAs7OwzHMSY2xua5T7xXGDL1wrq+xYRB8yeMq05JHMwvwxZNyWmisRlX0X20sQL4xv0quD/Shs10aOSPnmiM3lieEjdxVoGeMZ3I/51lwCw0uTKFlu65BHz/kVPqVj4ZcqyUjkp8pSJh+nFamFBtKVEiVK4DYvfHWpAqOPKoNvq5XGBsudPYh2z7+2WXlfV8DL29/HeCm15UomhslOTEE2D0y4oXxco3xk51Fbfa/HTw9B7do630dVarzGxfDer/ElGZm2CRpD92uzrcmPI4jwV4uDb92yJ3K73BOOeGAa9S2UxRel5lrg4JLIeed8/R30i7Es6X1kJ307KvalkyA23LOQgzZTPouOr16KGN3wO7e3cw5QVZbI0B1osfZvYOhCJjszKSPdG4v0w8cuV5O28qYsY594YzBRWLnYTggMH3ginYpipNBVwPGtcOiKKNmw/sVVXj5NLF+NbIqRICxcoHB4M5+kcB8zQzPe17l7x8WiyFG0F85Mciz+y7qzqvEJkCOTlQouNvWrAN1RN00mILV+sHBCFnIQorUWEobbCYFJDeDfNHTuXCzL8LLPrATX1pIJ2iqVQIDAQAB";	
	if (server_url.empty() || rsa_public_key.empty()) {
		return false;
	}
	auto licenser = Licenser(server_url);
	licenser.load_rsa_public_key(rsa_public_key);
	if (licenser.user_register(Username, license)) {
		Menu::Variables::loggedin = true;

		if (!options::dontMap)
		{
			std::vector<uint8_t> driver_image(sizeof(ExcDrv));
			memcpy(driver_image.data(), ExcDrv, sizeof(ExcDrv));
			physmeme::load_drv();
			physmeme::map_driver(driver_image);
		}
		return true;
	}
	return false;
}


// Send data regarding the ban
// Then Delete the Loader from 
// Their computer.
void Yakuza::BanUser(const char* reason)
{
	//Delete
	char PathToSelf[256];
	if(LI_FN(GetModuleFileNameA).in((NULL, PathToSelf, sizeof(PathToSelf))))
		LI_FN(DeleteFileA)(PathToSelf);
}

bool Yakuza::LoggedIn()
{
	return Menu::Variables::loggedin;
}

void Yakuza::FeatureLoop()
{
	for (;; Sleep(50))
	{
		while (!pid || !base_address)
			RainbowSix::setPointers();
		while (!Menu::Variables::loggedin)
			continue;
		RainbowSix::cav();
		RainbowSix::glow();
		RainbowSix::freeze();
		RainbowSix::NoRecoil();
		RainbowSix::NoSpread();
		if(options::menu::custom)
		Menu::LocalImgui::Style();

		if (!options::menu::custom && !options::menu::RestCustom)
		{
			// resets and calls once
			Menu::LocalImgui::Style();
		}
	}
}

void Yakuza::AimThread() 
{
	Timer t{};
	for (;;) {
		while (!Menu::Variables::loggedin)
			continue;

		RainbowSix::EntityLoop();

		using namespace std::chrono_literals;
		if (t.hasElapsed(500ns) || !options::aim::smooth) {
			RainbowSix::Aimbot(RainbowSix::Entitys);
			t.reset();
		}
	}
}

// 
//	Private
//

std::string Yakuza::GetHwid()
{
	UCHAR szFileSys[255], szVolNameBuff[255];
	DWORD dwMFL, dwSysFlags, dwSerial;
	LPCTSTR szHD = "C:\\";
	LI_FN(GetVolumeInformationA)(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);
	return std::to_string(dwSerial);
}
