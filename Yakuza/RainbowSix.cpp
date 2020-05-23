#include "RainbowSix.h"
#include "Offsets.hpp"
#include <iostream>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <thread>
#include <TlHelp32.h>
#include <thread>
#include <mutex>
#include "Vectors.h"
#include <xlocbuf>
#include <codecvt>

#define D3DXToRadian(degree) ((degree) * (M_PI / 180.0f))

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

namespace RainbowSix
{
	bool cav()
	{
		if (!options::cavEsp)
			return false;
	
		if (!gamemanager)
			return false;

		if (!lpEntityList || !GetEntityCount())
			return false;

		for (int i = 0; i < GetEntityCount(); i++)
		{
			uintptr_t entity_object = RPM<uintptr_t>(lpEntityList + i * 0x8);
			if (!entity_object) continue;

			uintptr_t chain2 = RPM<uintptr_t>(entity_object + 0x28); if (!chain2) continue;
			uintptr_t chain3 = RPM<uintptr_t>(chain2 + 0xD8); if (!chain3) continue;

			for (auto current_component = 0x80; current_component < 0xf0; current_component += sizeof(uintptr_t))
			{
				uintptr_t chain4 = RPM<uintptr_t>(chain3 + current_component);
				if (!chain4) continue;

				if (RPM<uintptr_t>(chain4) != (base_address + v_table_offset)) continue;

				WPM<BYTE>(chain4 + 0x552, (BYTE)options::cavEsp);
				WPM<BYTE>(chain4 + 0x554, (BYTE)options::cavEsp);
			}
		}
	}

	void NoRecoil()
	{
		if (!options::noRecoil)
			return;
		uintptr_t FOVManager = RPM<uintptr_t>(base_address + fov_manager_offset);
		if (!FOVManager)
			return;

		uintptr_t Recoil1 = RPM<uintptr_t>(FOVManager + 0x110); //110
		if (!Recoil1)
			return;

		uintptr_t Recoil2 = RPM<uintptr_t>(Recoil1); //Deref
		if (!Recoil2)
			return;
		//(BYTE)(0.1f)
		WPM<BYTE>(Recoil2 + 0xE2D, options::recoil); // E2D
	}

	void NoSpread() 
	{
		uintptr_t addr1 = RPM<uintptr_t>(gamemanager + 0xC8);
		uintptr_t addr2 = RPM<uintptr_t>(addr1 + 0x0);
		uintptr_t addr3 = RPM<uintptr_t>(addr2 + 0x90);
		uintptr_t addr4 = RPM<uintptr_t>(addr3 + 0xC8);
		uintptr_t addr5 = RPM<uintptr_t>(addr4 + 0x278);

		WPM<float>(addr5 + 0x58, options::noSpread);
	}

	void glow()
	{
		if (!options::glowEsp)
			return;

		uintptr_t base = RPM<uintptr_t>(base_address + glow_manager_offset);
		uintptr_t chain = RPM<uintptr_t>(base + glow_base);
		if (chain) {
			WPM<float>(chain + 0xD0, options::glow::red);
			WPM<float>(chain + 0xD4, options::glow::green);
			WPM<float>(chain + 0xD8, options::glow::blue);
			WPM<float>(chain + 0x118, -1.f); // opacity or maybe distance check in reclass
			WPM<float>(chain + 0x110 + 0x4, 255.f); // local
		}
	}

	void freeze() {
		uintptr_t FreezeManager = RPM<uintptr_t>(base_address + ADDRESS_FREEZEMANAGER);
		WPM<BYTE>(FreezeManager + 0x5c, options::freeze);
	}

	int getGameState() {
		uintptr_t addr = RPM<uintptr_t>(gamemanager + 0x20);
		uintptr_t addr1 = RPM<uintptr_t>(addr + 0x260);
		uintptr_t addr2 = RPM<uintptr_t>(addr1 + 0x50);
		uintptr_t addr3 = RPM<uintptr_t>(addr2 + 0x0);
		return RPM<int>(addr3 + 0x128);
	}

	uintptr_t GetEntity(int i)
	{
		uintptr_t entityBase = RPM<uintptr_t>(lpEntityList + (i * 0x8));
		return RPM<uintptr_t>(entityBase + 0x20);
	}

	uintptr_t GetEntityBase(int i)
	{
		return RPM<uintptr_t>(lpEntityList + (i * 0x8));
	}

	BYTE GetEntityTeamId(uintptr_t entity) {
		uintptr_t playerInfo = RPM<uintptr_t>(entity + 0xA8);
		uintptr_t playerInfoDeref = RPM<uintptr_t>(playerInfo);
		return RPM<BYTE>(playerInfoDeref + 0x19E);
	}

	Vector3 GetEntityFeetPosition(uintptr_t entity) {
		return Readclass<Vector3>(entity + OFFSET_ENTITY_FEET);
	}

	Vector3 GetEntityHeadPosition(uintptr_t entity) {
		//We get the head position straight from the entity
		return Readclass<Vector3>(entity + OFFSET_ENTITY_HEAD);
	}

	Vector3 GetEntityBonePos(uintptr_t entity) {
		//We get the head position straight from the entity
		return Readclass<Vector3>(entity + OFFSET_ENTITY_HEAD);
	}


	uintptr_t GetCamera() {
		return lpEngine;
	}
	//Camera pos
	Vector3 GetViewTranslation() {
		return Readclass<Vector3>(GetCamera() + translation);
	}

	Vector3 GetViewRight() {
		return Readclass<Vector3>(GetCamera() + ViewRight);
	}

	Vector3 GetViewUp() {
		return Readclass<Vector3>(GetCamera() + ViewUp);
	}

	Vector3 GetViewForward() {
		return Readclass<Vector3>(GetCamera() + ViewForward);
	}

	float GetFOVX() {
		uintptr_t fovLink = RPM<uintptr_t>(GetCamera() + 0x420);
		return RPM<float>(fovLink + fov_x);
	}

	float GetFOVY() {
		uintptr_t fovLink = RPM<uintptr_t>(GetCamera() + 0x420);
		return RPM<float>(fovLink +  fov_y);
	}

	Vector3 WorldToScreen(Vector3 position) {
		int displayWidth = GetSystemMetrics(SM_CXSCREEN);
		int displayHeight = GetSystemMetrics(SM_CYSCREEN);

		Vector3 temp = position - GetViewTranslation();

		float x = temp.Dot(GetViewRight());
		float y = temp.Dot(GetViewUp());
		float z = temp.Dot(GetViewForward() * -1);

		return Vector3((displayWidth / 2) * (1 + x / GetFOVX() / z), (displayHeight / 2) * (1 - y / GetFOVY() / z), z);
	}


	int GetEntityCount() {
		return (int)RPM<DWORD>(gamemanager + offset_entity_count) & 0x3fffffff;
	}

	std::string GetEntityName(uintptr_t entity)
	{
		wchar_t buf[32];
		uintptr_t pi;
		pi = RPM<uintptr_t>(entity + 0xA8);
		if (pi == 0x0)
			return "";
		uintptr_t pn;
		pn = RPM<uintptr_t>(pi + offset_playerinfo_name);
		if (pn == 0x0)
			return "";
		ReadArray(pn, buf, 32);

		std::wstring nigga = std::wstring(buf);
		std::string result = ws2s(nigga);
		return result;
	}

	//
	// Requires Entity Base
	//
	int GetEntityHealth(uintptr_t entity)
	{
		//Entity info pointer from the Entity
		uintptr_t EntityInfo = RPM<uintptr_t>(entity + 0x28);
		//Main component pointer from the entity info
		uintptr_t MainComponent = RPM<uintptr_t>(EntityInfo + 0xD8);
		//Child component pointer form the main component
		uintptr_t ChildComponent = RPM<uintptr_t>(MainComponent + 0x8);
		int health = RPM<uint8_t>(ChildComponent + 0x168);
		//Finally health from the child component
		return health;
	}

	PlayerInfo GetAllEntityInfo(uintptr_t entity, int i) {
		PlayerInfo p;

		p.EntHandle = entity;
		p.id = i;
		p.teamId = GetEntityTeamId(entity);
		p.distanceFromPlayer = GetEntityFeetPosition(GetEntity(0)).Distance(GetEntityFeetPosition(entity));
		p.Position = GetEntityFeetPosition(entity);
		p.w2sPos = WorldToScreen(p.Position);
		p.HeadPos = GetEntityHeadPosition(entity);
		p.w2sHead = WorldToScreen(p.HeadPos);
		p.ScrenTop = WorldToScreen(Vector3(p.HeadPos.x, p.HeadPos.y, p.HeadPos.z + 0.2));
		p.Health = GetEntityHealth(GetEntityBase(i));
		p.w2sName = WorldToScreen(Vector3(p.HeadPos.x, p.HeadPos.y, p.HeadPos.z + 0.1f));
		//p.Name = GetEntityName(entity);
		//p.w2sNeck = ;

		return p;
	}

	void EntityLoop()
	{
		std::vector<PlayerInfo> List;
		int entity_count = GetEntityCount();

		if (entity_count == NULL)
		{
			Entitys = List;
			return;
		}

		//entity_count++;
		for (int i = 0; i < entity_count; i++) {

			DWORD_PTR Entity = GetEntity(i);
			if (!Entity) continue;
			PlayerInfo Player = GetAllEntityInfo(Entity, i);
			List.push_back(Player);
		}
		Entitys = List;
	}

	inline float Length(Vector3 Vector)
	{
		return sqrt(Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z);
	}

	Vector3 CalcAngle(Vector3 enemypos, Vector3 camerapos)
	{
		float r2d = 57.2957795131f;
		Vector3 dir = enemypos - camerapos;

		float x = asin(dir.z / Length(dir)) * r2d;
		float z = atan(dir.y / dir.x) * r2d;

		if (dir.x >= 0.f) z += 180.f;
		if (x > 180.0f) x -= 360.f;
		else if (x < -180.0f) x += 360.f;

		return Vector3(x, 0.f, z + 90.f);
	}

	Vector4 CreateFromYawPitchRoll(float yaw, float pitch, float roll)
	{
		float cy = cos(yaw * 0.5);
		float sy = sin(yaw * 0.5);
		float cr = cos(roll * 0.5);
		float sr = sin(roll * 0.5);
		float cp = cos(pitch * 0.5);
		float sp = sin(pitch * 0.5);

		float w = cy * cr * cp + sy * sr * sp;
		float x = cy * sr * cp - sy * cr * sp;
		float y = cy * cr * sp + sy * sr * cp;
		float z = sy * cr * cp - cy * sr * sp;

		return Vector4(x, y, z, w);
	}

	Vector4 GetAngles()
	{
		uint64_t pSkeleton = RPM<uint64_t>(localplayer + 0x20);

		if (pSkeleton)
		{
			uint64_t viewAngle2 = RPM<uint64_t>(pSkeleton + 0x1200);
			return RPM<Vector4>(viewAngle2 + 0xC0);
		}
		return Vector4();
	}

	inline Vector3 toVec3(Vector4 data)
	{
		Vector3 ret;
		float r2d = 180 / M_PI;
		auto y2 = data.y * data.y;
		ret.z = std::atan2(2 * (data.w * data.z + data.x * data.y), (1 - 2 * (y2 + data.z * data.z))) * r2d;
		ret.x = std::atan2(2 * (data.w * data.x + data.y * data.z), (1 - 2 * (data.x * data.x + y2))) * r2d;
		ret.y = 0.f;
		return ret;
	}

	void SetAngles(Vector4 ang) {
		uint64_t pSkeleton = RPM<uint64_t>(localplayer + 0x20);
		if (pSkeleton)
		{
			uint64_t viewAngle2 = RPM<uint64_t>(pSkeleton + 0x1200);
			WPM<Vector4>(viewAngle2 + 0xC0, ang);
		}
	}

	void SetAngles(Vector3& TargetAngles)
	{
		float d2r = 0.01745329251f;
		TargetAngles.clamp();
		if (options::aim::smooth)
		{
			Vector3 CurrentAng = toVec3(GetAngles());
			Vector3 Diffrence = TargetAngles - CurrentAng;
			Diffrence /= Diffrence.Length();
			Diffrence /= options::aim::smooth;
			Vector3 NewAngles = Diffrence + CurrentAng;
			SetAngles(CreateFromYawPitchRoll(NewAngles.z * d2r, 0.f, NewAngles.x * d2r));
		}
		else
		{
			SetAngles(CreateFromYawPitchRoll(TargetAngles.z * d2r, 0.f, TargetAngles.x * d2r));
		}
	}

	bool Closest(const PlayerInfo& first, const PlayerInfo& second)
	{
		if (first.distanceFromPlayer < second.distanceFromPlayer) {
			return true;
		}
		else {
			return false;
		}
	}

	float GetFovToPlayer(float x, float y) {
		return sqrtf(powf(y, 2.0f) + powf(x, 2.0f));
	}

	void GetTarget(std::vector<PlayerInfo> ents, Vector3& ang) {
		Vector3 TempAngle;
		Vector3 CameraPos = GetViewTranslation();
		Vector3 TargetAngle;
		float distance = FLT_MAX;
		float tempdist = FLT_MAX;
		for (std::vector<PlayerInfo>::iterator it = ents.begin(); it != ents.end(); it++)
		{
			PlayerInfo Player = *it;
			//Check if player is on screen
			if (Player.w2sPos.z >= 0.1f && Player.w2sHead.z >= 0.1f)
			{
				tempdist = GetFovToPlayer(Player.w2sHead.x - ScreenX / 2, Player.w2sHead.y - ScreenY / 2);
				if (tempdist > options::aim::fov)
				{
					continue;
				}
				if (tempdist < distance)
				{
					distance = tempdist;
					TargetAngle = CalcAngle(Player.HeadPos, CameraPos);
				}
			}
		}
		ang = TargetAngle;
	}
	void Aimbot(std::vector<PlayerInfo> ents)
	{
		if (!options::aimbot || !localplayer)
			return;

		Vector3 AimAngle;

		GetTarget(ents, AimAngle);
		
		if (GetAsyncKeyState(options::aim::key) && AimAngle.x && AimAngle.z) 
		{
			SetAngles(AimAngle);
		}
	}

}


	
