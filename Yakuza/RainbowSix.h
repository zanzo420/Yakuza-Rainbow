#pragma once
#include "Vectors.h"
#include "Kernel.h"
#include "options.h"
#include "Offsets.hpp"
#include <vector>

namespace RainbowSix
{
	inline uintptr_t gamemanager;
	inline uintptr_t statusmanager;
	inline uintptr_t profilemanager;
	inline uintptr_t localplayer;
	inline uintptr_t lpEntityList;
	inline uintptr_t lpCamera;
	inline uintptr_t lpRender;
	inline uintptr_t lpEngineLink;
	inline uintptr_t lpEngine;

	static int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	static int ScreenY = GetSystemMetrics(SM_CYSCREEN);

	static uintptr_t GetLocalPlayer()
	{
		uintptr_t addr1 = RPM<uintptr_t>(profilemanager + 0x68);
		uintptr_t addr2 = RPM<uintptr_t>(addr1 + 0x0);
		return RPM<uintptr_t>(addr2 + 0x28);
	};

	static void setPointers() {\
		pid = GetPID("RainbowSix.exe");
		base_address = GetModuleBaseAddr("RainbowSix.exe", pid, GetModuleBase);
		gamemanager = RPM<uintptr_t>(base_address + game_manager_offset);
		statusmanager = RPM<uintptr_t>(base_address + status_manager_offset);
		profilemanager = RPM<uintptr_t>(base_address + profile_manager_offset);
		localplayer = GetLocalPlayer();
		lpEntityList = RPM<uintptr_t>(gamemanager + offset_entity_list);
		lpCamera = RPM<uintptr_t>(base_address + game_profile_offset); //game_profile is also contains a ptr to camera manager
		lpRender = RPM<uintptr_t>(lpCamera + 0x78);
		lpEngineLink = RPM<uintptr_t>(lpRender);
		lpEngine = RPM<uintptr_t>(lpEngineLink + 0x130);
	}

	static int screenX = GetSystemMetrics(SM_CXSCREEN);
	static int screenY = GetSystemMetrics(SM_CYSCREEN);

	bool cav();
	void glow();
	void freeze();
	void NoRecoil();
	void NoSpread();
	void Aimbot(std::vector<PlayerInfo> ents);

	inline std::vector<PlayerInfo> Entitys;
	void EntityLoop();

	int getGameState();

	
	uintptr_t GetEntity(int i);
	BYTE GetEntityTeamId(uintptr_t entity);
	Vector3 GetEntityFeetPosition(uintptr_t entity);
	Vector3 GetEntityHeadPosition(uintptr_t entity);

	BYTE GetEntityTeamId(uintptr_t entity);
	int GetEntityCount();

	Vector3 GetEntityBone(DWORD_PTR Entity, __int64 BoneID);
	PlayerInfo GetAllEntityInfo(uintptr_t entity, int i);

	Vector3 GetViewTranslation();
	Vector3 GetViewRight();
	Vector3 GetViewUp();
	Vector3 GetViewForward();
	float GetFOVX();
	float GetFOVY();
	Vector3 WorldToScreen(Vector3 position);


	Vector3 CalcAngle(Vector3 enemypos, Vector3 camerapos);
	Vector4 GetAngles();
	bool Closest(const PlayerInfo& first, const PlayerInfo& second);

}