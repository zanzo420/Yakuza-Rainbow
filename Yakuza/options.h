#pragma once

namespace options 
{
	inline float noSpread = 0.75f;
	inline bool cavEsp = false;
	inline bool glowEsp = false;
	namespace glow 
	{
		inline float red = 127.0f;
		inline float green = 0.0f;
		inline float blue = 0.0f;
		inline float alpha = 127.0f;
		inline float opacity = 255.0f;
		inline float distance = 1.f;
	}
	inline bool boxEsp = true;
	namespace esp 
	{
		inline bool skeleton = false;
		inline bool filled = false;
		inline bool snaplines = false;
		inline bool head = false;
		inline bool health = false;
		inline bool name = true;
		inline Vector3 color = { 0, 0, 255};
	}
	inline bool freeze = false;
	inline bool outline = false;
	inline bool noRecoil = false;
	inline float recoil = 0.1f;

	inline bool aimbot;
	namespace aim {
		inline int key;
		inline bool drawFov = false;
		inline float fov = 10.f;
		inline int smooth = 0;
		static const char* items[]{ "Head","Chest","Neck","Hand","Stomach","Pelvis" };
		inline int Selected = 0;
	}
}
