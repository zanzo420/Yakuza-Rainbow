#pragma once
#include <Windows.h>
#include "RainbowSix.h"
#include <string>
#include "string.hpp"
#include "importer.hpp"



static class Yakuza
{
public:
	Yakuza() {};
	~Yakuza() {};

	//
	//	Menu
	//
	static void MenuLoop();
	static void SetGameWindow();

	//
	//	Server
	//
	static bool Login(char* Username, char* license);
	static bool Register(char* Username, char* license);
	static void BanUser(const char* reason);

	//
	//	Cheat
	//
	static void FeatureLoop();
	static void AimThread();

private:
	static std::string GetHwid();

};

static Yakuza Cheat;