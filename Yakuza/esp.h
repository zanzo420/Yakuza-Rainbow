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

	if (options::aim::drawFov && options::aimbot) {
		Renderer::GetInstance()->DrawCircle(Vector2(displayWidth / 2, displayHeight / 2), options::aim::fov,
			!(options::aim::RainbowFov) ? DirectX::XMFLOAT4(options::aim::FovColor[0] * 255, options::aim::FovColor[1] * 255, options::aim::FovColor[2] * 255, options::aim::FovColor[3] * 255) : DirectX::XMFLOAT4(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2], 255.f), 1);
	}


	if (!options::EnemyEspBox && !options::EnemyEsp && !options::TeamEspBox && !options::TeamEsp || !RainbowSix::localplayer)
		return;
	
	for (std::vector<PlayerInfo>::iterator it = ents.begin(); it != ents.end(); ++it) {
		PlayerInfo Player = *it;
		// Player is on Screen

		Vector4 BoxESPColor;
		Vector4 FillBoxColor;
		Vector4 SnaplinesColor;
		Vector4 HeathColor;
		Vector4 NameColor;
		Vector4 OperatorNameColor;
		Vector4 BonezColor;
		Vector4 HeadCircleColor;
		Vector4 DoRainbowColorA = Vector4(options::esp::rainbowcolor[0] , options::esp::rainbowcolor[1] , options::esp::rainbowcolor[2], options::esp::rainbowcolor[3] * 255);;
		Vector3 DoRainbowColor = Vector3(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2]);;

		DirectX::XMFLOAT4 FilledEspBox;
		DirectX::XMFLOAT4 DoRainbowFillColor = DirectX::XMFLOAT4(options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2],150) ;


		if (!Player.onTeam)
		{
			BoxESPColor = Vector4(options::esp::EnemyBoxColor[0] * 255, options::esp::EnemyBoxColor[1] * 255, options::esp::EnemyBoxColor[2] * 255, options::esp::EnemyBoxColor[3] * 255);
			HeadCircleColor = Vector4(options::esp::EnemyHeadColor[0] * 255, options::esp::EnemyHeadColor[1] * 255, options::esp::EnemyHeadColor[2] * 255, options::esp::EnemyHeadColor[3] * 255);
			SnaplinesColor = Vector4(options::esp::EnemySnapLineColor[0] * 255, options::esp::EnemySnapLineColor[1] * 255, options::esp::EnemySnapLineColor[2] * 255, options::esp::EnemySnapLineColor[3] * 255);
			BonezColor = Vector4(options::esp::EnemySkeletonColor[0] * 255, options::esp::EnemySkeletonColor[1] * 255, options::esp::EnemySkeletonColor[2] * 255, options::esp::EnemySkeletonColor[3] * 255);
			NameColor = Vector4(options::esp::EnemyPlayerNameColor[0] * 255, options::esp::EnemyPlayerNameColor[1] * 255, options::esp::EnemyPlayerNameColor[2] * 255, options::esp::EnemyPlayerNameColor[3] * 255);
			OperatorNameColor = Vector4(options::esp::EnemyOperatorColor[0] * 255, options::esp::EnemyOperatorColor[1] * 255, options::esp::EnemyOperatorColor[2] * 255, options::esp::EnemyOperatorColor[3] * 255);
			FilledEspBox = DirectX::XMFLOAT4(options::esp::EnemyBoxColor[0] * 255, options::esp::EnemyBoxColor[1] * 255, options::esp::EnemyBoxColor[2] * 255, 150);
		}
		if (Player.onTeam)
		{
			BoxESPColor = Vector4(options::esp::TeamBoxColor[0] * 255, options::esp::TeamBoxColor[1] * 255, options::esp::TeamBoxColor[2] * 255, options::esp::TeamBoxColor[3] * 255);
			HeadCircleColor = Vector4(options::esp::TeamHeadColor[0] * 255, options::esp::TeamHeadColor[1] * 255, options::esp::TeamHeadColor[2] * 255, options::esp::TeamHeadColor[3] * 255);
			SnaplinesColor = Vector4(options::esp::TeamSnapLineColor[0] * 255, options::esp::TeamSnapLineColor[1] * 255, options::esp::TeamSnapLineColor[2] * 255, options::esp::TeamSnapLineColor[3] * 255);
			BonezColor = Vector4(options::esp::TeamSkeletonColor[0] * 255, options::esp::TeamSkeletonColor[1] * 255, options::esp::TeamSkeletonColor[2] * 255, options::esp::TeamSkeletonColor[3] * 255);
			NameColor = Vector4(options::esp::TeamPlayerNameColor[0] * 255, options::esp::TeamPlayerNameColor[1] * 255, options::esp::TeamPlayerNameColor[2] * 255, options::esp::TeamPlayerNameColor[3] * 255);
			OperatorNameColor = Vector4(options::esp::TeamOperatorColor[0] * 255, options::esp::TeamOperatorColor[1] * 255, options::esp::TeamOperatorColor[2] * 255, options::esp::TeamOperatorColor[3] * 255);
			FilledEspBox = DirectX::XMFLOAT4(options::esp::TeamBoxColor[0] * 255, options::esp::TeamBoxColor[1] * 255, options::esp::TeamBoxColor[2] * 255, 150);
		}
		if ((Player.w2sPos.z >= 0.1f && Player.w2sHead.z >= 0.1f)) {
			if (options::EnemyEspBox && options::EnemyEsp && !Player.onTeam)
			{
				switch (options::esp::EnemyType) {
					default:
					case 0: //Outline default
					{
						Renderer::GetInstance()->DrawOutlinedRect(Player.ScreenTop.x - Player.BoxWidth / 2,Player.ScreenTop.y,Player.BoxWidth, Player.BoxHeight, !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA);
						break;
					}
					case 1: 
					{
						Renderer::GetInstance()->DrawFilledRect(Player.ScreenTop.x - Player.BoxWidth / 2,Player.ScreenTop.y,Player.BoxWidth,Player.BoxHeight,!(options::esp::EnemyRainbowBox) ? FilledEspBox : DoRainbowFillColor);
						break;
					}
					case 2: 
					{
						float shift = 15;

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomRight.x, Player.BottomRight.y), Vector2(Player.BottomRight.x + shift, Player.BottomRight.y), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA ,2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomLeft.x, Player.BottomLeft.y), Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);
						
						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y), Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y - shift), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomRight.x + shift, Player.BottomRight.y),Vector2(Player.BottomRight.x + shift, Player.BottomRight.y - shift), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopRight.x, Player.TopRight.y), Vector2(Player.TopRight.x + shift, Player.TopRight.y), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopLeft.x, Player.TopLeft.y), Vector2(Player.TopLeft.x - shift, Player.TopLeft.y), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopLeft.x - shift, Player.TopLeft.y), Vector2(Player.TopLeft.x - shift, Player.TopLeft.y + shift), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopRight.x + shift, Player.TopRight.y), Vector2(Player.TopRight.x + shift, Player.TopRight.y + shift), !(options::esp::EnemyRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);
						break;
					}
					case 3:
					{
						Vector3 calculated_euler = calculate_euler(RainbowSix::GetAngles(Player.EntHandle));
						Renderer::GetInstance()->DrawBox3D(Player.Position, Vector4(options::esp::EnemyBoxColor[0], options::esp::EnemyBoxColor[1], options::esp::EnemyBoxColor[2], 255.f), 0.65f, RainbowSix::getmin(Player.EntBase), RainbowSix::getmax(Player.EntBase), calculated_euler);
						break;
					}
				}
			}
			if (options::TeamEsp && options::TeamEspBox && Player.onTeam)
			{
				switch (options::esp::TeamType) {
					default:
					case 0: //Outline default
					{
						Renderer::GetInstance()->DrawOutlinedRect(Player.ScreenTop.x - Player.BoxWidth / 2, Player.ScreenTop.y, Player.BoxWidth, Player.BoxHeight, !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA);
						break;
					}
					case 1:
					{
						Renderer::GetInstance()->DrawFilledRect(Player.ScreenTop.x - Player.BoxWidth / 2, Player.ScreenTop.y, Player.BoxWidth, Player.BoxHeight, !(options::esp::TeamRainbowBox) ? FilledEspBox : DoRainbowFillColor);
						break;
					}
					case 2:
					{
						float shift = 15;

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomRight.x, Player.BottomRight.y), Vector2(Player.BottomRight.x + shift, Player.BottomRight.y), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomLeft.x, Player.BottomLeft.y), Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y), Vector2(Player.BottomLeft.x - shift, Player.BottomLeft.y - shift), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.BottomRight.x + shift, Player.BottomRight.y), Vector2(Player.BottomRight.x + shift, Player.BottomRight.y - shift), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopRight.x, Player.TopRight.y), Vector2(Player.TopRight.x + shift, Player.TopRight.y), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopLeft.x, Player.TopLeft.y), Vector2(Player.TopLeft.x - shift, Player.TopLeft.y), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopLeft.x - shift, Player.TopLeft.y), Vector2(Player.TopLeft.x - shift, Player.TopLeft.y + shift), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);

						Renderer::GetInstance()->DrawLine(Vector2(Player.TopRight.x + shift, Player.TopRight.y), Vector2(Player.TopRight.x + shift, Player.TopRight.y + shift), !(options::esp::TeamRainbowBox) ? BoxESPColor : DoRainbowColorA, 2);
						break;
					}
					case 3:
					{
						Vector3 calculated_euler = calculate_euler(RainbowSix::GetAngles(Player.EntHandle));
						Renderer::GetInstance()->DrawBox3D(Player.Position, Vector4(options::esp::TeamBoxColor[0], options::esp::TeamBoxColor[1], options::esp::TeamBoxColor[2], 255.f), 0.65f, RainbowSix::getmin(Player.EntBase), RainbowSix::getmax(Player.EntBase), calculated_euler);
						break;
					}
				}
			}

			if (options::esp::EnemySnaplines && !Player.onTeam) 
			{
				Renderer::GetInstance()->DrawLine(Vector2(RainbowSix::screenX / 2, RainbowSix::screenY),Vector2(Player.w2sHead.x, Player.ScreenTop.y + Player.BoxHeight), !(options::esp::EnemySnapLineRainbow) ? SnaplinesColor : DoRainbowColorA, 2);
			}

			if (options::esp::TeamSnaplines && Player.onTeam)
			{
				Renderer::GetInstance()->DrawLine(Vector2(RainbowSix::screenX / 2, RainbowSix::screenY), Vector2(Player.w2sHead.x, Player.ScreenTop.y + Player.BoxHeight), !(options::esp::TeamSnapLineRainbow) ? SnaplinesColor : DoRainbowColorA, 2);
			}

			if (options::esp::EnemyHealth && !Player.onTeam) {
				Renderer::GetInstance()->DrawHealthbars((Player.ScreenTop.x - Player.BoxWidth / 2) - 5, Player.ScreenTop.y, Player.BoxHeight, Player.Health, Vector3(250, 65, 30));
			}

			if (options::esp::TeamHealth && Player.onTeam) {
				Renderer::GetInstance()->DrawHealthbars((Player.ScreenTop.x - Player.BoxWidth / 2) - 5, Player.ScreenTop.y, Player.BoxHeight, Player.Health, Vector3(250, 65, 30));
			}

			if (options::esp::EnemyHead && !Player.onTeam) {
				Renderer::GetInstance()->DrawCircleFilled(Vector2(Player.w2sHead.x, Player.w2sHead.y),Player.BoxHeight / options::esp::EnemyHeadSize,!(options::esp::EnemyHeadRainbow) ? HeadCircleColor : DoRainbowColorA);
			}

			if (options::esp::TeamHead && Player.onTeam) {
				Renderer::GetInstance()->DrawCircleFilled(Vector2(Player.w2sHead.x, Player.w2sHead.y), Player.BoxHeight / options::esp::EnemyHeadSize, !(options::esp::TeamHeadRainbow) ? HeadCircleColor : DoRainbowColorA);
			}

			if (options::esp::EnemySkeleton && !Player.onTeam) 
			{
				/*Neck-Chest*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sNeck.x, Player.w2sNeck.y), Vector2(Player.w2sChest.x, Player.w2sChest.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Chest-Stomach*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sChest.x, Player.w2sChest.y), Vector2(Player.w2sStomach.x, Player.w2sStomach.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Stomach-Pelvis*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sStomach.x, Player.w2sStomach.y), Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Pevlis-LKnee*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y),Vector2(Player.w2sLknee.x, Player.w2sLknee.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Pevlis-RKnee*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), Vector2(Player.w2sRknee.x, Player.w2sRknee.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*RKnee to Right foot*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sRknee.x, Player.w2sRknee.y), Vector2(Player.w2sRfoot.x, Player.w2sRfoot.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*LKnee to Left foot*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sLknee.x, Player.w2sLknee.y), Vector2(Player.w2sLfoot.x, Player.w2sLfoot.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Chest-right elbow*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sChest.x, Player.w2sChest.y), Vector2(Player.w2sRelbow.x, Player.w2sRelbow.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Chest-Left elbow*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sChest.x, Player.w2sChest.y), Vector2(Player.w2sLelbow.x, Player.w2sLelbow.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Left elbow to Lhand*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sLelbow.x, Player.w2sLelbow.y), Vector2(Player.w2sLHand.x, Player.w2sLHand.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Right Elbow to right hand*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sRelbow.x, Player.w2sRelbow.y), Vector2(Player.w2sRHand.x, Player.w2sRHand.y), !(options::esp::EnemySkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);
				
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

			if (options::esp::TeamSkeleton && Player.onTeam)
			{
				/*Neck-Chest*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sNeck.x, Player.w2sNeck.y), Vector2(Player.w2sChest.x, Player.w2sChest.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Chest-Stomach*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sChest.x, Player.w2sChest.y), Vector2(Player.w2sStomach.x, Player.w2sStomach.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Stomach-Pelvis*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sStomach.x, Player.w2sStomach.y), Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Pevlis-LKnee*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), Vector2(Player.w2sLknee.x, Player.w2sLknee.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Pevlis-RKnee*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sPelvis.x, Player.w2sPelvis.y), Vector2(Player.w2sRknee.x, Player.w2sRknee.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*RKnee to Right foot*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sRknee.x, Player.w2sRknee.y), Vector2(Player.w2sRfoot.x, Player.w2sRfoot.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*LKnee to Left foot*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sLknee.x, Player.w2sLknee.y), Vector2(Player.w2sLfoot.x, Player.w2sLfoot.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Chest-right elbow*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sChest.x, Player.w2sChest.y), Vector2(Player.w2sRelbow.x, Player.w2sRelbow.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Chest-Left elbow*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sChest.x, Player.w2sChest.y), Vector2(Player.w2sLelbow.x, Player.w2sLelbow.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Left elbow to Lhand*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sLelbow.x, Player.w2sLelbow.y), Vector2(Player.w2sLHand.x, Player.w2sLHand.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

				/*Right Elbow to right hand*/
				Renderer::GetInstance()->DrawLine(Vector2(Player.w2sRelbow.x, Player.w2sRelbow.y), Vector2(Player.w2sRHand.x, Player.w2sRHand.y), !(options::esp::TeamSkeletonRainbow) ? BonezColor : DoRainbowColorA, 2);

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


			if (options::esp::EnemyOperatorName && !Player.onTeam)
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.Name.c_str(), Vector2(Player.w2sName.x, Player.w2sName.y), options::esp::EnemyOperatorNameSize,!(options::esp::EnemyOperatorRainbow) ? OperatorNameColor : DoRainbowColorA,true);
			}

			if (options::esp::TeamOperatorName && Player.onTeam)
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.Name.c_str(), Vector2(Player.w2sName.x, Player.w2sName.y), options::esp::TeamOperatorNameSize, !(options::esp::TeamOperatorRainbow) ? OperatorNameColor : DoRainbowColorA, true);
			}

			if (options::esp::EnemyPlayer_name && !Player.onTeam)
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.PlayerName.c_str(),Vector2(Player.w2sPlayerName.x, Player.w2sPlayerName.y), options::esp::EnemyPlayerNameSize, !(options::esp::EnemyPlayerNameRainbow) ? NameColor : DoRainbowColorA, true);
			}

			if (options::esp::TeamPlayer_name && Player.onTeam)
			{
				Renderer::GetInstance()->DrawMyText(ImGui::GetFont(), (PCHAR)Player.PlayerName.c_str(), Vector2(Player.w2sPlayerName.x, Player.w2sPlayerName.y), options::esp::TeamPlayerNameSize, !(options::esp::TeamPlayerNameRainbow) ? NameColor : DoRainbowColorA, true);
			}

		}
	}

}