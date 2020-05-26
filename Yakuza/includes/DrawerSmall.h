#pragma once
#include "Imgui_Main.h"
#include "..\Vectors.h"
#include <d3d9.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct D3DTLVERTEX {
	float x, y, z, rhw;
	unsigned long color;
};

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

	float DrawMyText(ImFont* pFont, PCHAR text, const Vector2& pos, float size, Vector3 RGB, bool center);
	void DrawLine(const Vector2& from, const Vector2& to, Vector3 RGB, float thickness);
	void DrawHealthbars(float PosX, float PosY, float height, float Value1, Vector3 color);
	void DrawBox(const Vector2& from, const Vector2& size, Vector3 RGB, float rounding, float thickness);
	//void DrawBoxFilled(const Vector2& from, const Vector2& size, Vector3 color, float rounding);
	void DrawBoxFilled(const Vector2& from, const Vector2& size, Vector3 color, float rounding, float opacity = 255);
	void DrawClearBox(const Vector2& from, const Vector2& size, DirectX::XMFLOAT4 color, float rounding);
	void DrawBoxOutlined(const Vector2& from, const Vector2& size, Vector3 RGB, Vector3 OutlineRGB, float rounding, float thickness);
	void DrawCircle(const Vector2& from, float radius, DirectX::XMFLOAT4 color, float thickness);
	void DrawCircleFilled(const Vector2& from, float radius, Vector3 color);
	void DrawRect(float x, float y, float w, float h, Vector3 color);
	void DrawOutlinedRect(float x, float y, float w, float h, Vector3 color);
	void DrawFilledRect(float x, float y, float w, float h, Vector3 color);
	void DrawBox3D(Vector3 positon, Vector4 color, float thickness, Vector3 min, Vector3 max, Vector3 angels);
private:
	static Renderer* m_pInstance;
};