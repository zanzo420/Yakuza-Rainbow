#include <list>
#include "RainbowSix.h"
#include "Vectors.h"
#include "DrawerSmall.h"
#include <sstream>

inline std::wstring convert_int(int n)
{
	std::wstringstream ss;
	ss << std::hex << n;
	return ss.str();
}

static void BoxEsp(std::vector<PlayerInfo> ents) {

	//ImGuiIO& io = ImGui::GetIO();
	//ImFont* defaultFont = io.Fonts->AddFontDefault();

	if (!options::boxEsp || !RainbowSix::localplayer)
		return;
	
	for (std::vector<PlayerInfo>::iterator it = ents.begin(); it != ents.end(); ++it) {
		PlayerInfo Player = *it;

		float BoxHeight = Player.w2sPos.y - Player.ScreenTop.y;
		float BoxWidth = BoxHeight / 2.4;

		// Player is on Screen
		if (Player.w2sPos.z >= 0.1f) {
			if (options::boxEsp) {
				if (options::esp::filled) {
					Renderer::GetInstance()->DrawFilledRect(
						Player.ScreenTop.x - BoxWidth / 2,
						Player.ScreenTop.y, 
						BoxWidth, 
						BoxHeight, 
						Vector3(
						0,
						0,
						255));
				}
				else {
					Renderer::GetInstance()->DrawOutlinedRect(
						Player.ScreenTop.x - BoxWidth / 2, 
						Player.ScreenTop.y,
						BoxWidth, BoxHeight,
						Vector3(0, 167, 226));
				}
			}

			if (options::aim::drawFov) {
				Renderer::GetInstance()->DrawCircl(DirectX::XMFLOAT2(1920 / 2, 1080 / 2), options::aim::fov, DirectX::XMFLOAT4(250, 65, 30, 127), 1);
			}
			if (options::esp::snaplines) {
				Renderer::GetInstance()->DrawLine(
					DirectX::XMFLOAT2(RainbowSix::screenX / 2, RainbowSix::screenY),
					DirectX::XMFLOAT2(Player.w2sHead.x,Player.ScreenTop.y + BoxHeight), 
					Vector3(0,0,0), 
					1);
			}
			if (options::esp::health) {
				Renderer::GetInstance()->DrawHealthbars((Player.ScreenTop.x - BoxWidth / 2) - 5, Player.ScreenTop.y, BoxHeight, Player.Health, Vector3(250, 65, 30));
			}
			if (options::esp::head) {
				/*Head*/
				Renderer::GetInstance()->DrawCircleFilled(
					DirectX::XMFLOAT2(Player.w2sHead.x, Player.w2sHead.y),
					BoxHeight / 45.5,
					Vector3(0, 167, 226));
			}

			if (options::esp::skeleton) 
			{
				/*Neck-Chest*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sNeck.x, Player.w2sNeck.y), DirectX::XMFLOAT2(Player.w2sChest.x, Player.w2sChest.y), Vector3(0, 167, 226), 2);
				/*Chest-Stomach*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sChest.x, Player.w2sChest.y), DirectX::XMFLOAT2(Player.w2sStomach.x, Player.w2sStomach.y), Vector3(0, 167, 226), 2);
				/*Stomach-Pelvis*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sStomach.x, Player.w2sStomach.y), DirectX::XMFLOAT2(Player.w2sPelvis.x, Player.w2sPelvis.y), Vector3(0, 167, 226), 2);
				/*Pevlis-LKnee*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sPelvis.x, Player.w2sPelvis.y), DirectX::XMFLOAT2(Player.w2sLknee.x, Player.w2sLknee.y), Vector3(0, 167, 226), 2);
				/*Pevlis-RKnee*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sPelvis.x, Player.w2sPelvis.y), DirectX::XMFLOAT2(Player.w2sRknee.x, Player.w2sRknee.y), Vector3(0, 167, 226), 2);
				/*RKnee to Right foot*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sRknee.x, Player.w2sRknee.y), DirectX::XMFLOAT2(Player.w2sRfoot.x, Player.w2sRfoot.y), Vector3(0, 167, 226), 2);
				/*LKnee to Left foot*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sLknee.x, Player.w2sLknee.y), DirectX::XMFLOAT2(Player.w2sLfoot.x, Player.w2sLfoot.y), Vector3(0, 167, 226), 2);
				/*Chest-right elbow*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sChest.x, Player.w2sChest.y), DirectX::XMFLOAT2(Player.w2sRelbow.x, Player.w2sRelbow.y), Vector3(0, 167, 226), 2);
				/*Chest-Left elbow*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sChest.x, Player.w2sChest.y), DirectX::XMFLOAT2(Player.w2sLelbow.x, Player.w2sLelbow.y), Vector3(0, 167, 226), 2);
				/*Left elbow to Lhand*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sLelbow.x, Player.w2sLelbow.y), DirectX::XMFLOAT2(Player.w2sLHand.x, Player.w2sLHand.y), Vector3(0, 167, 226), 2);
				/*Right Elbow to right hand*/
				Renderer::GetInstance()->DrawLine(DirectX::XMFLOAT2(Player.w2sRelbow.x, Player.w2sRelbow.y), DirectX::XMFLOAT2(Player.w2sRHand.x, Player.w2sRHand.y), Vector3(0, 167, 226), 2);
				static int i = 0x0;
				if (GetAsyncKeyState(VK_END)) {
					i += 0x1;
					Sleep(10);
				}
				Vector3 loc = RainbowSix::WorldToScreen(RainbowSix::GetEntityBone(Player.EntHandle, i));
				const wchar_t* chr = convert_int(i).c_str();
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)chr, DirectX::XMFLOAT2(loc.x, loc.y), 12.f, Vector3(255, 255, 255), true);
				
			}

			if (options::esp::name) 
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.Name.c_str(), DirectX::XMFLOAT2(Player.w2sName.x, Player.w2sName.y), 24.f, Vector3(255,255,255), true);
			}
		}
	}

}