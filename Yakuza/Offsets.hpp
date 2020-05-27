/*

	Game offsets, top ones need to be updated every
	update generally, and for big updates other ones
	may need to be changed too.

*/
#pragma once

// RainbowSix.exe ->
constexpr uintptr_t game_manager_offset = 0x53beff0;
constexpr uintptr_t localplayerman_offset = 0x538E868;
constexpr uintptr_t fov_manager_offset = 0x53beff8;
constexpr uintptr_t profile_manager_offset = 0x53AC118;
constexpr uintptr_t renderer_manager_offset = 0x60a2688;
constexpr uintptr_t status_manager_offset = 0x60acd40;
constexpr uintptr_t network_manager_offset = 0x53ae880;
constexpr uintptr_t round_manager_offset = 0x53ae8b0;
constexpr uintptr_t glow_manager_offset = 0x60a2628;
constexpr uintptr_t skin_manager_offset = 0x605b1c0;
constexpr uintptr_t camrea_manager_offset = 0x53bee00;
constexpr uintptr_t game_profile_offset = 0x53AC118;
constexpr uintptr_t v_table_offset = 0x3acd590;
constexpr uintptr_t settings_manager_offset = 0x5383e60;

// Glow
constexpr uintptr_t glow_base = 0xb8;

// RainbowSix.exe -> game_manager -> 
constexpr uintptr_t offset_entity_count = 0xD0;
constexpr uintptr_t offset_entity_list = 0xC8;

// RainbowSix.exe -> game_manager -> entity_list ->
constexpr uintptr_t offset_entity_index = 0x8;
constexpr uintptr_t ADDRESS_FREEZEMANAGER = 0x53a8dd0; //48 8B 05 ? ? ? ? 0F B6 48 61 // 0x5c

// RainbowSix.exe -> camera-manager ->
constexpr uintptr_t ViewForward = 0x1E0;
constexpr uintptr_t ViewRight = 0x1C0;
constexpr uintptr_t ViewUp = 0x1D0;
constexpr uintptr_t fov_x = 0x800;
constexpr uintptr_t fov_y = 0x814;
constexpr uintptr_t translation = 0x1F0;

constexpr uintptr_t OFFSET_ENTITY_REF = 0x20;
constexpr uintptr_t OFFSET_ENTITY_HEAD = 0x6B0;
constexpr uintptr_t OFFSET_ENTITY_CHEST = 0xFB0;
constexpr uintptr_t OFFSET_ENTITY_FEET = 0x700;
constexpr uintptr_t offset_playerinfo_name = 0x1C8;

struct PlayerInfo {
	uintptr_t	EntHandle;
	uintptr_t	EntBase;
	int			id;
	int			teamId;
	bool		onTeam;
	
	float		distanceFromPlayer;
	float		Health;
	char		name[20];
	Vector3		Position;
	Vector3		ScreenTop;
	Vector3		w2sPos;
	Vector3		w2sName;
	Vector3		w2sPlayerName;

	std::string Name;
	std::string PlayerName;
	//Bones ----------------
	Vector3		HeadPos;
	Vector3		w2sHead;
	Vector3		w2sNeck;
	Vector3		w2sRHand;
	Vector3		w2sLHand;
	Vector3		w2sChest;
	Vector3		w2sStomach;
	Vector3		w2sPelvis;
	Vector3		w2sLelbow;
	Vector3		w2sRelbow;
	Vector3		w2sLknee;
	Vector3		w2sRknee;
	Vector3		w2sLfoot;
	Vector3		w2sRfoot;
	//Bones ----------------

	float BoxHeight;
	float BoxWidth;

	Vector2 TopLeft;
	Vector2 TopRight;
	Vector2 BottomLeft;
	Vector2 BottomRight;
	Vector2 BottomCenter;
	Vector2 TopCenter;
};