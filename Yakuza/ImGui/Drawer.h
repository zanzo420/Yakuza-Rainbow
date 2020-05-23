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

	Renderer();
	~Renderer();

	float DrawMyText(ImFont* pFont, PCHAR text, const DirectX::XMFLOAT2& pos, float size, Vector3 RGB, bool center);
	void DrawLine(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& to, Vector3 RGB, float thickness);
	void DrawBox(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& size, Vector3 RGB, float rounding, float thickness);
	void DrawBoxOutlined(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& size, Vector3 RGB, Vector3 OutlineRGB, float rounding, float thickness);
	void DrawBoxFilled(const DirectX::XMFLOAT2& from, const DirectX::XMFLOAT2& size, DirectX::XMFLOAT4 color, float rounding);
	void DrawCircle(const DirectX::XMFLOAT2& from, float radius, DirectX::XMFLOAT4 color, float thickness = 1.f);
	void DrawCircleFilled(const DirectX::XMFLOAT2& from, float radius, DirectX::XMFLOAT4 color);
private:
	static Renderer* m_pInstance;
};