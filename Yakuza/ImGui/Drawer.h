#pragma once
#include "imgui.h"
#include "imgui_internal.h"

#include <d3d11.h>
#include <dinput.h>
#include <tchar.h>
#include <Dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#include <Windows.h>
#include <Tlhelp32.h>
#include <string>
#include <mutex>
#include <windef.h>
#include <DirectXMath.h>
#include <cmath>
#include <windef.h>
#include <Psapi.h> 
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "psapi.lib")
#include "..\Vectors.h"
#pragma once
#include "imgui.h"
#include "imgui_internal.h"

#include <d3d11.h>
#include <dinput.h>
#include <tchar.h>
#include <Dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#include <Windows.h>
#include <Tlhelp32.h>
#include <string>
#include <mutex>
#include <windef.h>
#include <DirectXMath.h>
#include <cmath>
#include <windef.h>
#include <Psapi.h> 
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "psapi.lib")
#include "..\Vectors.h"

class Renderer
{
public:
	void Initialize();
	void BeginScene();
	void DrawScene();
	void EndScene();

	static Renderer* GetInstance();

	ImFont* m_pFont;
	ImFont* q_pFont;
	ImFont* l_pFont;

	Renderer();
	~Renderer();

	float DrawMyText(ImFont* pFont, PCHAR text, const Vector2& pos, float size, Vector3 RGB, bool center);
	void DrawLine(const Vector2& from, const Vector2& to, Vector3 RGB, float thickness);
	void DrawBox(const Vector2& from, const Vector2& size, Vector3 RGB, float rounding, float thickness);
	void DrawBoxOutlined(const Vector2& from, const Vector2& size, Vector3 RGB, Vector3 OutlineRGB, float rounding, float thickness);
	void DrawBoxFilled(const Vector2& from, const Vector2& size, DirectX::XMFLOAT4 color, float rounding);
	void DrawCircle(const Vector2& from, float radius, DirectX::XMFLOAT4 color, float thickness = 1.f);
	void DrawCircleFilled(const Vector2& from, float radius, DirectX::XMFLOAT4 color);
private:
	static Renderer* m_pInstance;
};