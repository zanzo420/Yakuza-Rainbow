#include <list>
#include "RainbowSix.h"
#include "Vectors.h"
#include "DrawerSmall.h"
#include <sstream>

inline std::string convert_int(int n)
{
	std::stringstream ss;
	ss << "0x" << std::hex << n;
	return ss.str();
}

Vector3 calculate_euler(Vector4 quaternion) {
	return Vector3(std::atan2(2.f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y), (1.f - 2.f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z))) * 57.2957795131f,
		std::atan2(1.9f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z), (1.f - 2.f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y))) * 57.2957795131f,
		0.f);
}

static void BoxEsp(std::vector<PlayerInfo> ents) {

	//ImGuiIO& io = ImGui::GetIO();
	//ImFont* defaultFont = io.Fonts->AddFontDefault();
	static int displayWidth;
	static int displayHeight;
	if (static std::atomic<bool> ran = false; !ran.exchange(true))
	{
		displayWidth = GetSystemMetrics(SM_CXSCREEN);
		displayHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	if (!options::boxEsp || !RainbowSix::localplayer)
		return;
	
	for (std::vector<PlayerInfo>::iterator it = ents.begin(); it != ents.end(); ++it) {
		PlayerInfo Player = *it;

		// Player is on Screen
		if ((Player.w2sPos.z >= 0.1f && Player.w2sHead.z >= 0.1f)) {
			if (options::boxEsp) {
				switch (options::esp::type) {
					default:
					case 0: //Outline default
					{
						Renderer::GetInstance()->DrawOutlinedRect(
							Player.ScreenTop.x - Player.BoxWidth / 2,
							Player.ScreenTop.y,
							Player.BoxWidth, Player.BoxHeight, 
							!(options::esp::RainbowBox) ? Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) : Vector3(options::esp::rainbowcolor[0] , options::esp::rainbowcolor[1] , options::esp::rainbowcolor[2]));
						break;
					}
					case 1: 
					{
						Renderer::GetInstance()->DrawFilledRect(
							Player.ScreenTop.x - Player.BoxWidth / 2,
							Player.ScreenTop.y,
							Player.BoxWidth,
							Player.BoxHeight,
							Vector3(
								0,
								0,
								255));
						break;
					}
					case 2: 
					{
						float shift = 15;
						Renderer::GetInstance()->DrawLine(
							Vector2(Player.BottomRight.x, Player.BottomRight.y), 
							Vector2(Player.BottomRight.x + shift, Player.BottomRight.y), !(options::esp::RainbowBox) ? 
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) : 
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]),2
						);

						Renderer::GetInstance()->DrawLine(
							Vector2(Player.BottomLeft.x, Player.BottomLeft.y), 
							Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);
						Renderer::GetInstance()->DrawLine(
							Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y), 
							Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y - shift), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);
						Renderer::GetInstance()->DrawLine(
							Vector2(Player.BottomRight.x + shift, Player.BottomRight.y),
							Vector2(Player.BottomRight.x + shift, Player.BottomRight.y - shift), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);

						Renderer::GetInstance()->DrawLine(
							Vector2(Player.TopRight.x, Player.TopRight.y), 
							Vector2(Player.TopRight.x + shift, Player.TopRight.y), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);
						Renderer::GetInstance()->DrawLine(
							Vector2(Player.TopLeft.x, Player.TopLeft.y), 
							Vector2(Player.TopLeft.x - shift, Player.TopLeft.y), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);
						Renderer::GetInstance()->DrawLine(
							Vector2(Player.TopLeft.x - shift, Player.TopLeft.y), 
							Vector2(Player.TopLeft.x - shift, Player.TopLeft.y + shift), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);
						Renderer::GetInstance()->DrawLine(
							Vector2(Player.TopRight.x + shift, Player.TopRight.y), 
							Vector2(Player.TopRight.x + shift, Player.TopRight.y + shift), !(options::esp::RainbowBox) ?
							Vector3(options::esp::BoxColor[0] * 255, options::esp::BoxColor[1] * 255, options::esp::BoxColor[2] * 255) :
							Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
						);
						break;
					}
					case 3:
					{
						Vector3 calculated_euler = calculate_euler(RainbowSix::GetAngles(Player.EntHandle));
						Renderer::GetInstance()->DrawBox3D(Player.Position, Vector4(options::esp::BoxColor[0], options::esp::BoxColor[1], options::esp::BoxColor[2], 255.f), 0.65f, RainbowSix::getmin(Player.EntBase), RainbowSix::getmax(Player.EntBase), calculated_euler);
						break;
					}
				}
			
			}

			if (options::aim::drawFov) {
				Renderer::GetInstance()->DrawCircle(Vector2(displayWidth / 2, displayHeight / 2), options::aim::fov, 
					!(options::aim::RainbowFov) ? DirectX::XMFLOAT4(options::aim::FovColor[0] * 255, options::aim::FovColor[1] * 255, options::aim::FovColor[2] * 255, 255.f) : DirectX::XMFLOAT4(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2], 255.f),1);
			}
			if (options::esp::snaplines) {
				Renderer::GetInstance()->DrawLine(
					Vector2(RainbowSix::screenX / 2, RainbowSix::screenY),
					Vector2(Player.w2sHead.x, Player.ScreenTop.y + Player.BoxHeight), !(options::esp::SnapLineRainbow) ?
					Vector3(options::esp::SnapLineColor[0] * 255, options::esp::SnapLineColor[1] * 255, options::esp::SnapLineColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
			}
			if (options::esp::health) {
				Renderer::GetInstance()->DrawHealthbars((Player.ScreenTop.x - Player.BoxWidth / 2) - 5, Player.ScreenTop.y, Player.BoxHeight, Player.Health, Vector3(250, 65, 30));
			}
			if (options::esp::head) {
				/*Head*/

				Renderer::GetInstance()->DrawCircleFilled(
					Vector2(Player.w2sHead.x, Player.w2sHead.y),
					Player.BoxHeight / options::esp::HeadSize,
					!(options::esp::HeadRainbow) ? Vector3(options::esp::HeadColor[0] * 255, options::esp::HeadColor[1] * 255, options::esp::HeadColor[2] * 255) : Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]));
			}

			if (options::esp::skeleton) 
			{
				/*Neck-Chest*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sNeck.x, Player.w2sNeck.y), 
					Vector2(Player.w2sChest.x, Player.w2sChest.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Chest-Stomach*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sChest.x, Player.w2sChest.y), 
					Vector2(Player.w2sStomach.x, Player.w2sStomach.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Stomach-Pelvis*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sStomach.x, Player.w2sStomach.y), 
					Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Pevlis-LKnee*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y),
					Vector2(Player.w2sLknee.x, Player.w2sLknee.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Pevlis-RKnee*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), 
					Vector2(Player.w2sRknee.x, Player.w2sRknee.y), 
					!(options::esp::SnapLineRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*RKnee to Right foot*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sRknee.x, Player.w2sRknee.y), 
					Vector2(Player.w2sRfoot.x, Player.w2sRfoot.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*LKnee to Left foot*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sLknee.x, Player.w2sLknee.y), 
					Vector2(Player.w2sLfoot.x, Player.w2sLfoot.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Chest-right elbow*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sChest.x, Player.w2sChest.y), 
					Vector2(Player.w2sRelbow.x, Player.w2sRelbow.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Chest-Left elbow*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sChest.x, Player.w2sChest.y), 
					Vector2(Player.w2sLelbow.x, Player.w2sLelbow.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Left elbow to Lhand*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sLelbow.x, Player.w2sLelbow.y), 
					Vector2(Player.w2sLHand.x, Player.w2sLHand.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				/*Right Elbow to right hand*/
				Renderer::GetInstance()->DrawLine(
					Vector2(Player.w2sRelbow.x, Player.w2sRelbow.y), 
					Vector2(Player.w2sRHand.x, Player.w2sRHand.y), 
					!(options::esp::SkeletonRainbow) ?
					Vector3(options::esp::SkeletonColor[0] * 255, options::esp::SkeletonColor[1] * 255, options::esp::SkeletonColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), 2
				);
				
				//for (int i = 0x0; i <= 0xff; i += 0x1)
				//{
				//	//static int i = 0x0;
				///*	if (GetAsyncKeyState(VK_END))
				//	{
				//		i += 0x1;
				//		Sleep(250);
				//	}*/
				//	Vector3 loc = RainbowSix::WorldToScreen(RainbowSix::GetEntityBone(Player.EntHandle, i));
				//	Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)convert_int(i).c_str(), Vector2(loc.x, loc.y), 16.f, Vector3(255, 255, 255), true);
				//}
			}

			if (options::esp::name) 
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.Name.c_str(), 
					Vector2(Player.w2sName.x, Player.w2sName.y), options::esp::OperatorNameSize, 
					!(options::esp::OperatorRainbow) ?
						Vector3(options::esp::OperatorColor[0] * 255, options::esp::OperatorColor[1] * 255, options::esp::OperatorColor[2] * 255) :
						Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]) ,true
				);
			}
			if (options::esp::player_name) 
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.PlayerName.c_str(), 
					Vector2(Player.w2sPlayerName.x, Player.w2sPlayerName.y), options::esp::PlayerNameSize,
					!(options::esp::PlayerNameRainbow) ?
					Vector3(options::esp::PlayerNameColor[0] * 255, options::esp::PlayerNameColor[1] * 255, options::esp::PlayerNameColor[2] * 255) :
					Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]), true
				);
			}
		}
	}

}