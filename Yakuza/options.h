#pragma once

namespace options 
{
	inline float noSpread = 0.75f;
	inline bool noSpreadEnabled = false;
	inline bool noSpreadDisabled = false;
	inline bool cavEsp = false;
	inline bool glowEsp = false;
	inline bool mapSpoof = false;

	namespace menu
	{
		inline bool custom = false;
		inline bool RestCustom = true;
		inline bool windowbg = false;
		inline bool windowbgchild = false;
		inline bool windowbar = false;
		inline bool CheckMark = false;
		inline bool Hover = false;
		inline bool RainbowWindowBg = false;
		inline bool RainbowWindowChildBg = false;
		inline bool RainbowWindowBar = false;
		inline bool RainbowCheckMark = false;
		inline bool RainbowHover = false;
		inline float ImGuiCol_TitleBg[4] = { 0.f * 255, 0.f * 255, 0.f * 255, 1.f *255 };
		inline float ImGuiCol_WindowBg[4] = { 0.f * 255, 0.f * 255, 0.f * 255, 1.f * 255 };
		inline float ImGuiCol_ChildWindowBg[4] = { 0.f * 255, 0.f * 255, 0.f * 255, 1.f * 255 };
		inline float ImGuiCol_CheckMark[4] = { 1.f * 255, 1.f * 255, 1.f * 255, 1.f * 255 };
		inline float ImGuiCol_ButtonHovered[4] = { 1.f * 255, 1.f * 255, 1.f * 255, 1.f * 255 };
	}

	namespace glow 
	{
		inline float red = 127.0f;
		inline float green = 0.0f;
		inline float blue = 0.0f;
		inline float alpha = 127.0f;
		inline float opacity = 255.0f;
		inline float distance = 1.f;
		inline bool rainbow = false;
		inline bool GlowRainbow = false;
	}
	inline bool EnemyEsp = true;
	inline bool TeamEsp = false;
	inline bool EnemyEspBox = true;
	inline bool TeamEspBox = false;
	namespace esp
	{
		inline bool EnemySkeleton = false;
		inline int EnemyType = 0;
		inline int EspTeams = 0;
		inline bool EnemySnaplines = false;
		inline bool EnemyHead = false;
		inline bool EnemyHealth = false;
		inline bool EnemyPlayer_name = false;
		inline bool EnemyOperatorName = false;
		inline bool EnemyRainbowBox = false;
		inline bool EnemyHeadRainbow = false;
		inline bool GlowRainbow = false;
		inline bool EnemySnapLineRainbow = false;
		inline bool EnemySkeletonRainbow = false;
		inline bool EnemyPlayerNameRainbow = false;
		inline bool EnemyOperatorRainbow = false;
		inline float EnemyBox_width = 2.f;
		inline float EnemyHeadSize = 45.5;
		inline float EnemyPlayerNameSize = 12.f;
		inline float EnemyOperatorNameSize =12.f;
		inline float EnemyBoxColor[4] = { 255.0f, 0.0f, 0.0f, 0.9f };
		inline float EnemyHeadColor[4] = { 255.0f, 0.0f, 0.0f, 0.9f };
		inline float rainbowcolor[4] = { 255.f, 0.f, 0.f,0.9f };
		inline float EnemySnapLineColor[4] = { 255.f, 0.f, 0.f, 0.9f };
		inline float EnemySkeletonColor[4] = { 255.f, 0.f, 0.f, 0.9f };
		inline float EnemyOperatorColor[4] = { 255.f, 255.f, 255.f, 0.9f };
		inline float EnemyPlayerNameColor[4] = { 255.f, 255.f, 255.f, 0.9f };


		inline bool TeamSkeleton = false;
		inline int	TeamType = 0;
		inline bool TeamSnaplines = false;
		inline bool TeamHead = false;
		inline bool TeamHealth = false;
		inline bool TeamPlayer_name = false;
		inline bool TeamOperatorName = false;
		inline bool TeamRainbowBox = false;
		inline bool TeamHeadRainbow = false;
		inline bool TeamSnapLineRainbow = false;
		inline bool TeamSkeletonRainbow = false;
		inline bool TeamPlayerNameRainbow = false;
		inline bool TeamOperatorRainbow = false;
		inline float TeamBox_width = 2.f;
		inline float TeamHeadSize = 45.5;
		inline float TeamPlayerNameSize = 12.f;
		inline float TeamOperatorNameSize = 12.f;
		inline float TeamBoxColor[4] = { 255.0f, 0.0f, 0.0f, 0.9f };
		inline float TeamHeadColor[4] = { 255.0f, 0.0f, 0.0f, 0.9f };
		inline float TeamSnapLineColor[4] = { 255.f, 0.f, 0.f, 0.9f };
		inline float TeamSkeletonColor[4] = { 255.f, 0.f, 0.f, 0.9f };
		inline float TeamOperatorColor[4] = { 255.f, 255.f, 255.f, 0.9f };
		inline float TeamPlayerNameColor[4] = { 255.f, 255.f, 255.f, 0.9f };

	}

	inline bool freeze = false;
	inline bool outline = false;
	inline bool noRecoilEnabled = false;
	inline bool noRecoilDisabled = true;

	inline float recoil = 0.1f;

	inline bool aimbot;
	namespace aim {
		inline int key;
		inline bool drawFov = false;
		inline bool RainbowFov = false;
		inline float FovColor[4] = { 255.0f, 0.0f, 0.0f, 0.9f };
		inline float fov = 10.f;
		inline int smooth = 0;
		static const char* items[]{ "Head","Chest","Neck","Hand","Stomach","Pelvis" };
		inline int Selected = 0;
	}
}
