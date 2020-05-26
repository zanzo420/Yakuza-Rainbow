#pragma once

namespace options 
{
	inline float noSpread = 0.75f;
	inline bool noSpreadEnabled = false;
	inline bool noSpreadDisabled = false;
	inline bool cavEsp = false;
	inline bool glowEsp = false;
	inline bool mapSpoof = false;
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
	inline bool boxEsp = true;
	namespace esp 
	{
		inline bool skeleton = false;
		inline int type = 0;
		inline bool snaplines = false;
		inline bool head = false;
		inline bool health = false;
		inline bool player_name = false;
		inline bool name = false;
		inline bool RainbowBox = false;
		inline bool HeadRainbow = false;
		inline bool GlowRainbow = false;
		inline bool SnapLineRainbow = false;
		inline bool SkeletonRainbow = false;
		inline bool PlayerNameRainbow = false;
		inline bool OperatorRainbow = false;
		inline float box_width = 2.f;
		inline float HeadSize = 45.5;
		inline float PlayerNameSize = 12.f;
		inline float OperatorNameSize =12.f;
		inline float BoxColor[3] = { 255.0f, 0.0f, 0.0f };
		inline float HeadColor[3] = { 255.0f, 0.0f, 0.0f };
		inline float rainbowcolor[3] = { 255.f, 0.f, 0.f };
		inline float SnapLineColor[3] = { 255.f, 0.f, 0.f };
		inline float SkeletonColor[3] = { 255.f, 0.f, 0.f };
		inline float OperatorColor[3] = { 255.f, 255.f, 255.f };
		inline float PlayerNameColor[3] = { 255.f, 255.f, 255.f };

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
		inline float FovColor[3] = { 255.0f, 0.0f, 0.0f };
		inline float fov = 10.f;
		inline int smooth = 0;
		static const char* items[]{ "Head","Chest","Neck","Hand","Stomach","Pelvis" };
		inline int Selected = 0;
	}
}
