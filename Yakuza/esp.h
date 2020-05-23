#include <list>
#include "RainbowSix.h"
#include "Vectors.h"
#include "DrawerSmall.h"

static void BoxEsp(std::vector<PlayerInfo> ents) {

	//ImGuiIO& io = ImGui::GetIO();
	//ImFont* defaultFont = io.Fonts->AddFontDefault();

	if (!options::boxEsp || !RainbowSix::localplayer)
		return;
	
	for (std::vector<PlayerInfo>::iterator it = ents.begin(); it != ents.end(); ++it) {
		PlayerInfo Player = *it;

		float BoxHeight = Player.w2sPos.y - Player.ScrenTop.y;
		float BoxWidth = BoxHeight / 2.4;

		// Player is on Screen
		if (Player.w2sPos.z >= 0.1f && Player.w2sHead.z >= 0.1f) {
			if (options::boxEsp) {
				if (options::esp::filled) {
					Renderer::GetInstance()->DrawFilledRect(
						Player.ScrenTop.x - BoxWidth / 2,
						Player.ScrenTop.y, 
						BoxWidth, 
						BoxHeight, 
						Vector3(
						0,
						0,
						255));
				}
				else {
					Renderer::GetInstance()->DrawOutlinedRect(
						Player.ScrenTop.x - BoxWidth / 2, Player.ScrenTop.y,
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
					DirectX::XMFLOAT2(Player.w2sHead.x,Player.ScrenTop.y + BoxHeight), 
					Vector3(0,0,0), 
					1);
			}
			if (options::esp::health) {
				Renderer::GetInstance()->DrawHealthbars((Player.ScrenTop.x - BoxWidth / 2) - 5, Player.ScrenTop.y, BoxHeight, Player.Health, Vector3(250, 65, 30));
			}
			if (options::esp::head) {
				/*Head*/
				Renderer::GetInstance()->DrawCircleFilled(
					DirectX::XMFLOAT2(Player.w2sHead.x, Player.w2sHead.y),
					BoxHeight / 45.5,
					Vector3(0, 167, 226));
			}

			if (options::esp::name) 
			{
				//Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.Name.c_str(), DirectX::XMFLOAT2(Player.w2sName.x, Player.w2sName.y), 12.f, Vector3(255,255,255), false);
			}
		}
	}

}