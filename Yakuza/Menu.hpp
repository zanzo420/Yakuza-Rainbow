#pragma once
#include <Windows.h>
#include "options.h"
#include "DrawerSmall.h"
#include "Vectors.h"
#include "string.hpp"
#include "importer.hpp"
#include "esp.h"
#include "lists.h"
#include "imgui_impl_dx11.h"
#include <random>

#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
// text
#define TEXT_i(v) ImVec4(0.860f, 0.930f, 0.890f, v)

namespace Menu
{	
	namespace Variables
	{
		static float FPSLock = 144.f;
		static bool loggedin = false;
		static bool bMenu = true;
		static bool init = false;
		static int selected = 0;
		static float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static bool WindowStatus = true;
		static inline HWND WindowHandle;
		static inline WNDCLASSEX WindowClass;
		static inline HWND DESKTOP;
		static inline HWND PrevWindow;

	}

	static ID3D11Device* g_pd3dDevice = NULL;
	static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	static IDXGISwapChain* g_pSwapChain = NULL;
	static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	namespace LocalImgui 
	{
		static void ChangeClickAbility(bool canclick, HWND CHWND)
		{
			long style = GetWindowLong(CHWND, GWL_EXSTYLE);
			if (canclick)
			{
				style &= ~WS_EX_LAYERED;
				SetWindowLong(CHWND, GWL_EXSTYLE, style);
				SetForegroundWindow(CHWND);
			}
			else
			{
				style |= WS_EX_LAYERED;
				SetWindowLong(CHWND, GWL_EXSTYLE, style);
			}
		}

		static DWORD dwLastFrameTime = 0;
		static BOOLEAN LockFPS(INT FPS)
		{
			if (FPS == 0)
				FPS = 500;

			SYSTEMTIME st;
			GetSystemTime(&st);

			INT dwCurrentTime = st.wMilliseconds;
			if ((dwCurrentTime - dwLastFrameTime) < (1000 / FPS)) {
				return TRUE;
			}
			dwLastFrameTime = dwCurrentTime;

			return FALSE;
		}

		static void CreateRenderTarget()
		{
			DXGI_SWAP_CHAIN_DESC sd;
			g_pSwapChain->GetDesc(&sd);

			ID3D11Texture2D* pBackBuffer;
			D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
			ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
			render_target_view_desc.Format = sd.BufferDesc.Format;
			render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
			pBackBuffer->Release();
		}

		static void CleanupRenderTarget()
		{
			if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
		}

		static HRESULT CreateDeviceD3D(HWND hWnd)
		{
			DXGI_SWAP_CHAIN_DESC sd;
			{
				ZeroMemory(&sd, sizeof(sd));
				sd.BufferCount = 2;
				sd.BufferDesc.Width = 0;
				sd.BufferDesc.Height = 0;
				sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				sd.BufferDesc.RefreshRate.Numerator = 0;
				sd.BufferDesc.RefreshRate.Denominator = 1;
				sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				sd.OutputWindow = hWnd;
				sd.SampleDesc.Count = 1;
				sd.SampleDesc.Quality = 0;
				sd.Windowed = TRUE;
				sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			}

			UINT createDeviceFlags = 0;
			D3D_FEATURE_LEVEL featureLevel;
			const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0, };
			if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
				return E_FAIL;

			CreateRenderTarget();

			return S_OK;
		}

		static void CleanupDeviceD3D()
		{
			CleanupRenderTarget();
			if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
			if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
			if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
		}

		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
				return true;

			switch (msg)
			{
			case WM_SIZE:
				if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
				{
					ImGui_ImplDX11_InvalidateDeviceObjects();
					CleanupRenderTarget();
					g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
					CreateRenderTarget();
					ImGui_ImplDX11_CreateDeviceObjects();
				}
				return 0;
			case WM_SYSCOMMAND:
				if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
					return 0;
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		static Vector3 GetClientsDisplayRes()
		{
			RECT Desktop;
			Vector3 Resolution;
			GetWindowRect(GetDesktopWindow(), &Desktop);
			Resolution.x = Desktop.right;
			Resolution.y = Desktop.bottom;
			return Resolution;
		}

		static void Style() {
			ImVec4* colors = ImGui::GetStyle().Colors;
			ImGuiStyle& style = ImGui::GetStyle();
			if (!options::menu::custom )
			{
				colors[ImGuiCol_Text] = ImVec4(255.f, 255.f, 255.f, 1.00f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
				colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

				colors[ImGuiCol_WindowBg] = ImVec4(0.125f, 0.125f, 0.149f, 1.00f);
				colors[ImGuiCol_ChildBg] = ImVec4(0.105f, 0.105f, 0.133f, 1.00f);
				colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
				colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_BorderShadow] = ImVec4(0.070f, 0.070f, 0.101f, 1.00f);

				colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_FrameBgHovered] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				colors[ImGuiCol_FrameBgActive] = ImVec4(0.862f, 0.862f, 0.862f, .50f);

				colors[ImGuiCol_TitleBg] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
				colors[ImGuiCol_TitleBgActive] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);

				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.917, 0.223, 0.223, .25);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.917, 0.223, 0.223, .25);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10, 0.10, 0.10, 1.00);

				colors[ImGuiCol_Separator] = ImVec4(0.917, 0.223, 0.223, .25f);
				colors[ImGuiCol_SeparatorActive] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				colors[ImGuiCol_SeparatorHovered] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);

				colors[ImGuiCol_SliderGrab] = ImVec4(0.862f, 0.862f, 0.862f, .90f);
				colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);

				colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);

				// List Box

				colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_HeaderHovered] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);

				colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
				colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
				colors[ImGuiCol_ResizeGripActive] = ImVec4(0.862f, 0.862f, 0.862f, .90f);

				colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
				colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
				colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
				colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
				colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
				options::menu::RestCustom = true;
			}
			else
			{
				colors[ImGuiCol_Text] = ImVec4(255.f, 255.f, 255.f, 1.00f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
				colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
				if (options::menu::windowbg)
				{
					colors[ImGuiCol_WindowBg] = !options::menu::RainbowWindowBg ? ImVec4(options::menu::ImGuiCol_WindowBg[0], options::menu::ImGuiCol_WindowBg[1], options::menu::ImGuiCol_WindowBg[2], options::menu::ImGuiCol_WindowBg[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, 1.00f);
				}
				else
				{
					colors[ImGuiCol_WindowBg] = ImVec4(0.125f, 0.125f, 0.149f, 1.00f);
				}

				if (options::menu::windowbgchild)
				{
					colors[ImGuiCol_ChildBg] = !options::menu::RainbowWindowChildBg ? ImVec4(options::menu::ImGuiCol_ChildWindowBg[0], options::menu::ImGuiCol_ChildWindowBg[1], options::menu::ImGuiCol_ChildWindowBg[2], options::menu::ImGuiCol_ChildWindowBg[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, 1.00f);
				}
				else
				{
					colors[ImGuiCol_ChildBg] = ImVec4(0.105f, 0.105f, 0.133f, 1.00f);
				}
				colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
				colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_BorderShadow] = ImVec4(0.070f, 0.070f, 0.101f, 1.00f);

				colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_FrameBgActive] = ImVec4(0.862f, 0.862f, 0.862f, .50f);

				if (options::menu::windowbar)
				{
					colors[ImGuiCol_TitleBg] = !options::menu::RainbowWindowBar ? ImVec4(options::menu::ImGuiCol_TitleBg[0], options::menu::ImGuiCol_TitleBg[1], options::menu::ImGuiCol_TitleBg[2], options::menu::ImGuiCol_TitleBg[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, options::menu::ImGuiCol_WindowBg[3]);
					colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

					colors[ImGuiCol_TitleBgActive] = !options::menu::RainbowWindowBar ? ImVec4(options::menu::ImGuiCol_TitleBg[0], options::menu::ImGuiCol_TitleBg[1], options::menu::ImGuiCol_TitleBg[2], options::menu::ImGuiCol_TitleBg[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, options::menu::ImGuiCol_WindowBg[3]);
					colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
				}
				else
				{
					colors[ImGuiCol_TitleBg] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
					colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
					colors[ImGuiCol_TitleBgActive] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				}

				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.917, 0.223, 0.223, .25);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.917, 0.223, 0.223, .25);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10, 0.10, 0.10, 1.00);

				colors[ImGuiCol_Separator] = ImVec4(0.917, 0.223, 0.223, .25f);
				colors[ImGuiCol_SeparatorActive] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				colors[ImGuiCol_SeparatorHovered] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);

				if (options::menu::CheckMark)
				{
					colors[ImGuiCol_CheckMark] = !options::menu::RainbowCheckMark ? ImVec4(options::menu::ImGuiCol_CheckMark[0], options::menu::ImGuiCol_CheckMark[1], options::menu::ImGuiCol_CheckMark[2], options::menu::ImGuiCol_CheckMark[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, 1.00f);
				}
				else
				{
					colors[ImGuiCol_CheckMark] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				}
				colors[ImGuiCol_SliderGrab] = ImVec4(0.862f, 0.862f, 0.862f, .90f);
				colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);

				colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				if (options::menu::Hover)
				{
					colors[ImGuiCol_ButtonHovered] = !options::menu::RainbowHover ? ImVec4(options::menu::ImGuiCol_ButtonHovered[0], options::menu::ImGuiCol_ButtonHovered[1], options::menu::ImGuiCol_ButtonHovered[2], options::menu::ImGuiCol_ButtonHovered[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, 1.00f);

					colors[ImGuiCol_FrameBgHovered] = !options::menu::RainbowHover ? ImVec4(options::menu::ImGuiCol_ButtonHovered[0], options::menu::ImGuiCol_ButtonHovered[1], options::menu::ImGuiCol_ButtonHovered[2], options::menu::ImGuiCol_ButtonHovered[3]) :
						ImVec4(options::esp::rainbowcolor[0] / 255, options::esp::rainbowcolor[1] / 255, options::esp::rainbowcolor[2] / 255, 1.00f);
				}
				else
				{
					colors[ImGuiCol_ButtonHovered] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
					colors[ImGuiCol_FrameBgHovered] = ImVec4(0.929f, 0.290f, 0.290f, 1.00f);
				}

				// List Box

				colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_HeaderHovered] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);

				colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
				colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
				colors[ImGuiCol_ResizeGripActive] = ImVec4(0.862f, 0.862f, 0.862f, .90f);

				colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.231f, 1.00f);
				colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
				colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
				colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
				colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
				colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
				options::menu::RestCustom = false;
			}


			style.WindowPadding = ImVec2(6, 4);
			style.WindowRounding = 0.0f;
			style.FramePadding = ImVec2(5, 2);
			style.FrameRounding = 3.0f;
			style.ItemSpacing = ImVec2(7, 1);
			style.ItemInnerSpacing = ImVec2(1, 1);
			style.TouchExtraPadding = ImVec2(0, 0);
			style.IndentSpacing = 6.0f;
			style.ScrollbarSize = 12.0f;
			style.ScrollbarRounding = 16.0f;
			style.GrabMinSize = 20.0f;
			style.GrabRounding = 2.0f;

			
		}
		static inline std::string window_name;
		static HWND GetWindowHandle()
		{
			
			std::generate_n(std::back_inserter(window_name), 16, []()
				{
					thread_local std::mt19937_64 mersenne_generator(std::random_device{ }());
					const std::uniform_int_distribution<> distribution(97, 122);
					return static_cast<std::uint8_t>(distribution(mersenne_generator));
				});
			Variables::WindowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, LocalImgui::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T(window_name.c_str()), NULL };
			RegisterClassEx(&Variables::WindowClass);
			HWND MenuHandle = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, _T(window_name.c_str()), _T(window_name.c_str()), WS_POPUP | WS_CHILD, 0, 0, LocalImgui::GetClientsDisplayRes().x - 1, LocalImgui::GetClientsDisplayRes().y - 1, NULL, NULL, Variables::WindowClass.hInstance, NULL);

			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(MenuHandle, &margins);

			if (LocalImgui::CreateDeviceD3D(MenuHandle) < 0)
			{
				LocalImgui::CleanupDeviceD3D();
				UnregisterClass(_T(window_name.c_str()), Variables::WindowClass.hInstance);
				return 0;
			}

			ImGui::CreateContext();
			ImGui_ImplDX11_Init(MenuHandle, g_pd3dDevice, g_pd3dDeviceContext);
			LocalImgui::ChangeClickAbility(true, MenuHandle);

			Renderer::GetInstance()->Initialize();
			ImGuiIO& io = ImGui::GetIO();
			ImFont* imFont = io.Fonts->AddFontDefault();
			Style();

			Menu::Variables::init = true;
			return MenuHandle;
		}

		static void ImShutdown()
		{
			ImGui_ImplDX11_Shutdown();
			ImGui::DestroyContext();
			LocalImgui::CleanupDeviceD3D();
			UnregisterClass(_T(window_name.c_str()), Variables::WindowClass.hInstance);
			Menu::Variables::init = false;
		}
	}

	void rainbow_color()
	{
		if (options::esp::rainbowcolor[0] == 255 && options::esp::rainbowcolor[1] < 255 && options::esp::rainbowcolor[2] == 0) {
			options::esp::rainbowcolor[1]++;
		}
		else if (options::esp::rainbowcolor[0] <= 255 && options::esp::rainbowcolor[0] > 0 && options::esp::rainbowcolor[1] == 255 && options::esp::rainbowcolor[2] == 0) {
			options::esp::rainbowcolor[0]--;
		}
		else if (options::esp::rainbowcolor[0] == 0 && options::esp::rainbowcolor[1] == 255 && options::esp::rainbowcolor[2] >= 0 && options::esp::rainbowcolor[2] < 255) {
			options::esp::rainbowcolor[2]++;
		}
		else if (options::esp::rainbowcolor[0] == 0 && options::esp::rainbowcolor[1] <= 255 && options::esp::rainbowcolor[1] > 0 && options::esp::rainbowcolor[2] == 255) {
			options::esp::rainbowcolor[1]--;
		}
		else if (options::esp::rainbowcolor[0] >= 0 && options::esp::rainbowcolor[0] < 255 && options::esp::rainbowcolor[1] == 0 && options::esp::rainbowcolor[2] == 255) {
			options::esp::rainbowcolor[0]++;
		}
		else if (options::esp::rainbowcolor[0] == 255 && options::esp::rainbowcolor[1] == 0 && options::esp::rainbowcolor[2] > 0 && options::esp::rainbowcolor[2] <= 255) {
			options::esp::rainbowcolor[2]--;
		}
	}

	static void Esp()
	{
		ImGui::BeginGroup();

		//Space to left Top Tab
		ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
		ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();


		ImGui::Combo(xorstr_(" Team Selector"), &options::esp::EspTeams, EspTeams, IM_ARRAYSIZE(EspTeams));
		if (options::esp::EspTeams == 0)
		{
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::Checkbox(xorstr_(" Enemy"), &options::EnemyEsp);
			if (options::EnemyEsp)
			{
				static ImColor RainbowCycle_ImColor = ImColor{ options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2],  0.9f };
				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

				ImGui::Combo(xorstr_(" Box Type"), &options::esp::EnemyType, BoxType, IM_ARRAYSIZE(BoxType));

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupEnemyBoxColor = ImGui::ColorButton(("colorButtonEnemyBoxColor"),
					options::esp::EnemyRainbowBox ? RainbowCycle_ImColor : ImColor{ options::esp::EnemyBoxColor[0], options::esp::EnemyBoxColor[1], options::esp::EnemyBoxColor[2], options::esp::EnemyBoxColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupEnemyBoxColor)
					ImGui::OpenPopup(("EnemyBoxColorPopup"));
				if (ImGui::BeginPopup(("EnemyBoxColorPopup"))) {
					ImGui::ColorPicker4(("ESP Box Color"), options::esp::EnemyBoxColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Enemy_BoxESP"), &options::esp::EnemyRainbowBox);
					ImGui::SameLine();
					ImGui::SliderFloat(("Box Width"), &options::esp::EnemyBox_width, 0.1, 4.0);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Enemy ESP"), &options::EnemyEspBox);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupEnemyHeadColor = ImGui::ColorButton(("colorButtonEnemyHeadColor"),
					options::esp::EnemyHeadRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::EnemyHeadColor[0], options::esp::EnemyHeadColor[1], options::esp::EnemyHeadColor[2], options::esp::EnemyHeadColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupEnemyHeadColor)
					ImGui::OpenPopup(("EnemyHeadColorPopup"));
				if (ImGui::BeginPopup(("EnemyHeadColorPopup"))) {
					ImGui::ColorPicker4(("Head Color"), options::esp::EnemyHeadColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Enemy_HeadColor"), &options::esp::EnemyHeadRainbow);
					ImGui::SameLine();
					ImGui::SliderFloat(("Head Size"), &options::esp::EnemyHeadSize, 50.0, 10.0);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Head ESP"), &options::esp::EnemyHead);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupEnemySnapLineColor = ImGui::ColorButton(("colorButtonEnemySnapLineColor"),
					options::esp::EnemySnapLineRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::EnemySnapLineColor[0], options::esp::EnemySnapLineColor[1], options::esp::EnemySnapLineColor[2], options::esp::EnemySnapLineColor[3]}, ImGuiColorEditFlags_NoTooltip);
				if (openPopupEnemySnapLineColor)
					ImGui::OpenPopup(("EnemySnapLineColorPopup"));
				if (ImGui::BeginPopup(("EnemySnapLineColorPopup"))) {
					ImGui::ColorPicker4(("ESP Box Color"), options::esp::EnemySnapLineColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Enemy_SnapLine"), &options::esp::EnemySnapLineRainbow);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Snaplines"), &options::esp::EnemySnaplines);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupEnemySkeletonColor = ImGui::ColorButton(("colorButtonEnemySkeletonColor"),
					options::esp::EnemySkeletonRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::EnemySkeletonColor[0], options::esp::EnemySkeletonColor[1], options::esp::EnemySkeletonColor[2], options::esp::EnemySkeletonColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupEnemySkeletonColor)
					ImGui::OpenPopup(("EnemySkeletonColorPopup"));
				if (ImGui::BeginPopup(("EnemySkeletonColorPopup"))) {
					ImGui::ColorPicker4(("Skeleton Color"), options::esp::EnemySkeletonColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Enemy_Skeleton"), &options::esp::EnemySkeletonRainbow);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Skelton ESP"), &options::esp::EnemySkeleton);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupEnemyPlayerNameColor = ImGui::ColorButton(("colorButtonEnemyPlayerNameColor"),
					options::esp::EnemyPlayerNameRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::EnemyPlayerNameColor[0], options::esp::EnemyPlayerNameColor[1], options::esp::EnemyPlayerNameColor[2], options::esp::EnemyPlayerNameColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupEnemyPlayerNameColor)
					ImGui::OpenPopup(("EnemyPlayerNameColorPopup"));
				if (ImGui::BeginPopup(("EnemyPlayerNameColorPopup"))) {
					ImGui::ColorPicker4(("Player Name Color"), options::esp::EnemyPlayerNameColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Enemy_PlayerName"), &options::esp::EnemyPlayerNameRainbow);
					ImGui::SameLine();
					ImGui::SliderFloat(("Font Size"), &options::esp::EnemyPlayerNameSize, 5.0f, 20.0f);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Player Names"), &options::esp::EnemyPlayer_name);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupEnemyOperatorColor = ImGui::ColorButton(("colorButtonOperatorColor"),
					options::esp::EnemyOperatorRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::EnemyOperatorColor[0], options::esp::EnemyOperatorColor[1], options::esp::EnemyOperatorColor[2], options::esp::EnemyOperatorColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupEnemyOperatorColor)
					ImGui::OpenPopup(("EnemyOperatorColorPopup"));
				if (ImGui::BeginPopup(("EnemyOperatorColorPopup"))) {
					ImGui::ColorPicker4(("Operator Color"), options::esp::EnemyOperatorColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Enemy_OperatorColor"), &options::esp::EnemyOperatorRainbow);
					ImGui::SameLine();
					ImGui::SliderFloat(("Font Size"), &options::esp::EnemyOperatorNameSize, 5.0f, 20.0f);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox((" Operator Names"), &options::esp::EnemyOperatorName);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

				ImGui::SameLine(29);
				ImGui::Checkbox(xorstr_(" Health"), &options::esp::EnemyHealth);

			}
		}
		if (options::esp::EspTeams == 1)
		{
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::Checkbox(xorstr_(" Team ESP"), &options::TeamEsp);
			if (options::TeamEsp)
			{
				static ImColor RainbowCycle_ImColor = ImColor{ options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2],  0.9f };

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				ImGui::Combo(xorstr_(" Box Type"), &options::esp::TeamType, BoxType, IM_ARRAYSIZE(BoxType));

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupTeamBoxColor = ImGui::ColorButton(("colorButtonTeamBoxColor"),
					options::esp::TeamRainbowBox ? RainbowCycle_ImColor : ImColor{ options::esp::TeamBoxColor[0], options::esp::TeamBoxColor[1], options::esp::TeamBoxColor[2], options::esp::TeamBoxColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupTeamBoxColor)
					ImGui::OpenPopup(("TeamBoxColorPopup"));
				if (ImGui::BeginPopup(("TeamBoxColorPopup"))) {
					ImGui::ColorPicker4(("ESP Box Color"), options::esp::TeamBoxColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Team_BoxESP"), &options::esp::TeamRainbowBox);
					ImGui::SameLine();
					ImGui::SliderFloat(("Box Width"), &options::esp::TeamBox_width, 0.1, 4.0);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Team ESP"), &options::TeamEspBox);


				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupTeamHeadColor = ImGui::ColorButton(("colorButtonTeamHeadColor"),
					options::esp::TeamHeadRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::TeamHeadColor[0], options::esp::TeamHeadColor[1], options::esp::TeamHeadColor[2], options::esp::TeamHeadColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupTeamHeadColor)
					ImGui::OpenPopup(("TeamHeadColorPopup"));
				if (ImGui::BeginPopup(("TeamHeadColorPopup"))) {
					ImGui::ColorPicker4(("Head Color"), options::esp::TeamHeadColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Team_HeadColor"), &options::esp::TeamHeadRainbow);
					ImGui::SameLine();
					ImGui::SliderFloat(("Head Size"), &options::esp::TeamHeadSize, 50.0, 10.0);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Head ESP"), &options::esp::TeamHead);


				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupTeamSnapLineColor = ImGui::ColorButton(("colorButtonTeamSnapLineColor"),
					options::esp::TeamSnapLineRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::TeamSnapLineColor[0], options::esp::TeamSnapLineColor[1], options::esp::TeamSnapLineColor[2], options::esp::TeamSnapLineColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupTeamSnapLineColor)
					ImGui::OpenPopup(("TeamSnapLineColorPopup"));
				if (ImGui::BeginPopup(("TeamSnapLineColorPopup"))) {
					ImGui::ColorPicker4(("ESP Box Color"), options::esp::TeamSnapLineColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Team_SnapLine"), &options::esp::TeamSnapLineRainbow);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Snaplines"), &options::esp::TeamSnaplines);


				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupTeamSkeletonColor = ImGui::ColorButton(("colorButtonTeamSkeletonColor"),
					options::esp::TeamSkeletonRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::TeamSkeletonColor[0], options::esp::TeamSkeletonColor[1], options::esp::TeamSkeletonColor[2], options::esp::TeamSkeletonColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupTeamSkeletonColor)
					ImGui::OpenPopup(("TeamSkeletonColorPopup"));
				if (ImGui::BeginPopup(("TeamSkeletonColorPopup"))) {
					ImGui::ColorPicker4(("Skeleton Color"), options::esp::TeamSkeletonColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Team_Skeleton"), &options::esp::TeamSkeletonRainbow);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Skelton ESP"), &options::esp::TeamSkeleton);


				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupTeamPlayerNameColor = ImGui::ColorButton(("colorButtonTeamPlayerNameColor"),
					options::esp::TeamPlayerNameRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::TeamPlayerNameColor[0], options::esp::TeamPlayerNameColor[1], options::esp::TeamPlayerNameColor[2], options::esp::TeamPlayerNameColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupTeamPlayerNameColor)
					ImGui::OpenPopup(("TeamPlayerNameColorPopup"));
				if (ImGui::BeginPopup(("TeamPlayerNameColorPopup"))) {
					ImGui::ColorPicker4(("Player Name Color"), options::esp::TeamPlayerNameColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Team_PlayerName"), &options::esp::TeamPlayerNameRainbow);
					ImGui::SameLine();
					ImGui::SliderFloat(("Font Size"), &options::esp::TeamPlayerNameSize, 5.0f, 20.0f);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox(xorstr_(" Player Names"), &options::esp::TeamPlayer_name);


				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
				bool openPopupTeamOperatorColor = ImGui::ColorButton(("colorButtonOperatorColor"),
					options::esp::TeamOperatorRainbow ? RainbowCycle_ImColor : ImColor{ options::esp::TeamOperatorColor[0], options::esp::TeamOperatorColor[1], options::esp::TeamOperatorColor[2], options::esp::TeamOperatorColor[3] }, ImGuiColorEditFlags_NoTooltip);
				if (openPopupTeamOperatorColor)
					ImGui::OpenPopup(("TeamOperatorColorPopup"));
				if (ImGui::BeginPopup(("TeamOperatorColorPopup"))) {
					ImGui::ColorPicker4(("Operator Color"), options::esp::TeamOperatorColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
					ImGui::Checkbox(("Rainbow##RainbowCylcle_Team_OperatorColor"), &options::esp::TeamOperatorRainbow);
					ImGui::SameLine();
					ImGui::SliderFloat(("Font Size"), &options::esp::TeamOperatorNameSize, 5.0f, 20.0f);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Checkbox((" Operator Names"), &options::esp::TeamOperatorName);

				ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

				ImGui::SameLine(29);
				ImGui::Checkbox(xorstr_(" Health"), &options::esp::TeamHealth);

			}
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Checkbox(xorstr_(" Cav ESP"), &options::cavEsp);
		ImGui::EndGroup();
	}

	static void Misc() 
	{
		ImGui::BeginGroup();

		//Space to left Top Tab
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Checkbox(xorstr_(" No Recoil"), &options::noRecoilEnabled);
		if (options::noRecoilEnabled)
		{

			ImGui::SliderFloat(xorstr_( "No Recoil"), &options::recoil, 0.0f, 0.75000f);
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Checkbox(xorstr_(" No Spread"), &options::noSpreadEnabled);

		if (options::noSpreadEnabled)
		{
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::SliderFloat(xorstr_(" Spread"), &options::noSpread, 0.0f, 0.75000f);
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		if (ImGui::Button(xorstr_(" Reload")))
			RainbowSix::setPointers();

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Separator();
		//ImGui::Checkbox(xorstr_("Freeze"), &options::freeze);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		if (ImGui::Button(xorstr_("Exit"))) {
			LI_FN(exit)(0);
		}
		ImGui::EndGroup();
	}

	static void Glow() 
	{
		ImGui::BeginGroup();
		//Space to left Top Tab
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Checkbox(xorstr_(" Glow"), &options::glowEsp);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Checkbox(xorstr_(" Rainbow"), &options::glow::GlowRainbow);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::SliderFloat(xorstr_(" Glow R "), &options::glow::red, 0.f, 255.f);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::SliderFloat(xorstr_(" Glow G "), &options::glow::green, 0.f, 255.f);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::SliderFloat(xorstr_(" Glow B "), &options::glow::blue, 0.f, 255.f);


		ImGui::EndGroup();
	}

	static void Aim() 
	{
		ImGui::BeginGroup();
		//Space to left Top Tab
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Checkbox(xorstr_(" Aimbot"), &options::aimbot);
		if (options::aimbot)
		{
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::SliderFloat(xorstr_(" Fov"), &options::aim::fov, 0.f, 100.f);

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::Checkbox(xorstr_(" Draw Fov"), &options::aim::drawFov);

			ImGui::SameLine();
			static ImColor RainbowCycle_ImColor = ImColor{ options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2],  0.9f };

			bool openPopupFOVColor = ImGui::ColorButton(("colorButtonFOVColor"),
				options::aim::RainbowFov ? RainbowCycle_ImColor : ImColor{ options::aim::FovColor[0], options::aim::FovColor[1], options::aim::FovColor[2], options::aim::FovColor[3]}, ImGuiColorEditFlags_NoTooltip);
			if (openPopupFOVColor)
				ImGui::OpenPopup(("FOVColorPopup"));
			if (ImGui::BeginPopup(("FOVColorPopup"))) {
				ImGui::ColorPicker4(("FOV Color"), options::aim::FovColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
				ImGui::Checkbox(("Rainbow##RainbowCylcle_FOVColor"), &options::aim::RainbowFov);
				ImGui::EndPopup();
			}

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::SliderInt(xorstr_(" Smooth"), &options::aim::smooth, 1, 100);

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			ImGui::Combo(xorstr_(" Aim Key"), &options::aim::key, AllKeys, IM_ARRAYSIZE(AllKeys));
		}

		ImGui::EndGroup();
	}

	static void MenuSettings()
	{
		ImGui::BeginGroup();
		//Space to left Top Tab
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::Checkbox(xorstr_(" Customize"), &options::menu::custom);
		if (options::menu::custom)
		{
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			static ImColor RainbowCycle_ImColor = ImColor{ options::esp::rainbowcolor[0], options::esp::rainbowcolor[1], options::esp::rainbowcolor[2],  255.f };

			bool openPopupWindowBarColor = ImGui::ColorButton(("colorButtonMenuWindowColor"),
				options::menu::RainbowWindowBar ? RainbowCycle_ImColor : ImColor{ options::menu::ImGuiCol_TitleBg[0], options::menu::ImGuiCol_TitleBg[1], options::menu::ImGuiCol_TitleBg[2],options::menu::ImGuiCol_TitleBg[3]}, ImGuiColorEditFlags_NoTooltip);
			if (openPopupWindowBarColor)
				ImGui::OpenPopup(("WindowBarColorPopup"));
			if (ImGui::BeginPopup(("WindowBarColorPopup"))) {
				ImGui::ColorPicker4(("Window Bar Color"), options::menu::ImGuiCol_TitleBg, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
				ImGui::Checkbox(("Rainbow##RainbowCylcle_WindowBar"), &options::menu::RainbowWindowBar);
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Checkbox(xorstr_(" Menu Bar Color"), &options::menu::windowbar);

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			bool openPopupWindowBGColor = ImGui::ColorButton(("colorButtonMenuWindowBGColor"),
				options::menu::RainbowWindowBg ? RainbowCycle_ImColor : ImColor{ options::menu::ImGuiCol_WindowBg[0], options::menu::ImGuiCol_WindowBg[1], options::menu::ImGuiCol_WindowBg[2],options::menu::ImGuiCol_WindowBg[3]}, ImGuiColorEditFlags_NoTooltip);
			if (openPopupWindowBGColor)
				ImGui::OpenPopup(("WindowBGColorPopup"));
			if (ImGui::BeginPopup(("WindowBGColorPopup"))) {
				ImGui::ColorPicker4(("Window BG Color"), options::menu::ImGuiCol_WindowBg, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
				ImGui::Checkbox(("Rainbow##RainbowCylcle_WindowBG"), &options::menu::RainbowWindowBg);
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Checkbox(xorstr_(" Menu Bg Color"), &options::menu::windowbg);

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			bool openPopupWindowBGChildColor = ImGui::ColorButton(("colorButtonMenuWindowBG2Color"),
				options::menu::RainbowWindowChildBg ? RainbowCycle_ImColor : ImColor{ options::menu::ImGuiCol_ChildWindowBg[0], options::menu::ImGuiCol_ChildWindowBg[1], options::menu::ImGuiCol_ChildWindowBg[2],options::menu::ImGuiCol_ChildWindowBg[3] }, ImGuiColorEditFlags_NoTooltip);
			if (openPopupWindowBGChildColor)
				ImGui::OpenPopup(("WindowBGChildColorPopup"));
			if (ImGui::BeginPopup(("WindowBGChildColorPopup"))) {
				ImGui::ColorPicker4(("Window BG Color 2"), options::menu::ImGuiCol_ChildWindowBg, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
				ImGui::Checkbox(("Rainbow##RainbowCylcle_WindowBGChild"), &options::menu::RainbowWindowChildBg);
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Checkbox(xorstr_(" Menu BG 2 Color"), &options::menu::windowbgchild);

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			bool openPopupMenuHoverColor = ImGui::ColorButton(("colorButtonMenuHoverColor"),
				options::menu::RainbowHover ? RainbowCycle_ImColor : ImColor{ options::menu::ImGuiCol_ButtonHovered[0], options::menu::ImGuiCol_ButtonHovered[1], options::menu::ImGuiCol_ButtonHovered[2],options::menu::ImGuiCol_ButtonHovered[3] }, ImGuiColorEditFlags_NoTooltip);
			if (openPopupMenuHoverColor)
				ImGui::OpenPopup(("MenuHoverColorPopup"));
			if (ImGui::BeginPopup(("MenuHoverColorPopup"))) {
				ImGui::ColorPicker4(("Menu Hover Color"), options::menu::ImGuiCol_ButtonHovered, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
				ImGui::Checkbox(("Rainbow##RainbowCylcle_MenuHover"), &options::menu::RainbowHover);
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Checkbox(xorstr_(" Hover Color"), &options::menu::Hover);
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			bool openPopupMenuCheckMarkColor = ImGui::ColorButton(("colorButtonMenuCheckMarkColor"),
				options::menu::RainbowCheckMark ? RainbowCycle_ImColor : ImColor{ options::menu::ImGuiCol_CheckMark[0], options::menu::ImGuiCol_CheckMark[1], options::menu::ImGuiCol_CheckMark[2],options::menu::ImGuiCol_CheckMark[3] }, ImGuiColorEditFlags_NoTooltip);
			if (openPopupMenuCheckMarkColor)
				ImGui::OpenPopup(("MenuCheckMarkColorPopup"));
			if (ImGui::BeginPopup(("MenuCheckMarkColorPopup"))) {
				ImGui::ColorPicker4(("Menu Text Color"), options::menu::ImGuiCol_CheckMark, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB);
				ImGui::Checkbox(("Rainbow##RainbowCylcle_MenuCheckMark"), &options::menu::RainbowCheckMark);
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Checkbox(xorstr_(" Check Mark Color"), &options::menu::CheckMark);
		}

		ImGui::EndGroup();
	}


	static void Menu()
	{
		ImGui::SetNextWindowSize(ImVec2(700, 400));
		ImGui::Begin(xorstr_("Yakuza"), &Variables::bMenu, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGuiIO& io = ImGui::GetIO();

		// This is my example of font initializing
		// First font used for UI
		// Second for rendering, 32.0f is pixel size, not font size.
		ImGui::Spacing();
		ImGui::BeginChild("##tabs", ImVec2(130, 368));
		{
			ImGui::SameLine(18);
			ImGui::TextColored(ImVec4(127.f, 33.f, 33.f, 127.f), xorstr_("Yakuza R.4.0.2"));
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
						
			ImGui::Dummy(ImVec2(0, 0));
			if (ImGui::Button(xorstr_("A"), ImVec2(130, 40)))
				Variables::selected = 0;
			ImGui::NewLine();
			if (ImGui::Button(xorstr_("B"), ImVec2(130, 40)))
				Variables::selected = 1;
			ImGui::NewLine();
			if (ImGui::Button(xorstr_("C"), ImVec2(130, 40)))
				Variables::selected = 2;
			ImGui::NewLine();
			if (ImGui::Button(xorstr_("D"), ImVec2(130, 40)))
				Variables::selected = 3;
			ImGui::NewLine();
			if (ImGui::Button(xorstr_("E"), ImVec2(130, 40)))
				Variables::selected = 4;
			ImGui::PopFont();
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginGroup();
		{
			switch (Variables::selected)
			{
			case 0:
				Aim();
				break;
			case 1:
				Glow();
				break;
			case 2:
				Esp();
				break;
			case 3:
				Misc();
				break;
			case 4:
				MenuSettings();
				break;
			}
		}

		
		ImGui::EndGroup();
		ImGui::End();
	}

	static void Login()
	{
		ImGui::SetNextWindowSize(ImVec2(250, 280));
		ImGui::Begin(xorstr_(" Login "), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SameLine(60); ImGui::TextColored(ImVec4(127.f, 33.f, 33.f, 127.f), xorstr_("A"));
		ImGui::PopFont();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		static char username[512];
		static char password[512];

		ImGui::SameLine(50); ImGui::InputTextEx(xorstr_("  "), "", username, IM_ARRAYSIZE(username), ImVec2(150, 20), NULL);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("username");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("     "); ImGui::SameLine(50); ImGui::InputTextEx(xorstr_(" "), "", password, IM_ARRAYSIZE(password), ImVec2(150, 20), NULL);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("license");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		ImGui::SameLine(50);
		if (ImGui::Button(xorstr_(" Login "))) {
			Yakuza::Login(&username[0], &password[0]);
		}
		ImGui::SameLine();
		if (ImGui::Button(xorstr_("Register"))) {
			Yakuza::Register(&username[0], &password[0]);
		}
		ImGui::SameLine();

		//if (ImGui::Button(xorstr_("Debug Login")))
		//Menu::Variables::loggedin = true;


		if (ImGui::Button(xorstr_("Exit"))) {
			exit(0);
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SameLine(50);
		ImGui::Checkbox(xorstr_("  Spoof HWID"), &options::mapSpoof);
		ImGui::Checkbox(xorstr_("  Don't Map"), &options::dontMap);
		ImGui::End();
	}

	static bool IntializeMenu()
	{
		static MSG msg;
		//std::cout << sizeof(msg) << std::endl;
		if (!Variables::init) 
		{
			ZeroMemory(&msg, sizeof(msg));
		}
		static HWND MenuWindowHandle = NULL;
		if (!Variables::init)
		{
			MenuWindowHandle = LocalImgui::GetWindowHandle();
		}

		ShowWindow(MenuWindowHandle, SW_SHOWNORMAL);
		if (msg.message == WM_QUIT)
		{
			LocalImgui::ImShutdown();
		}

		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			return 0;
		}

		ImGui_ImplDX11_NewFrame();

		if (
				//Menu Customizations
				options::menu::RainbowWindowBg || options::menu::RainbowWindowBar || options::menu::RainbowWindowChildBg || options::menu::RainbowCheckMark || options::menu::RainbowHover ||
				//Glow RainBow  bool Start
				options::glow::rainbow ||
				//EnemyRainBow bool Start
				options::esp::EnemyRainbowBox || options::esp::EnemyHeadRainbow || options::aim::RainbowFov || options::esp::EnemySnapLineRainbow || options::esp::EnemySkeletonRainbow || options::esp::EnemyOperatorRainbow || options::esp::EnemyPlayerNameRainbow ||
				//TeamRainBow bool Start
				options::esp::TeamRainbowBox  || options::esp::TeamHeadRainbow || options::aim::RainbowFov || options::esp::TeamSnapLineRainbow || options::esp::TeamSkeletonRainbow || options::esp::TeamOperatorRainbow || options::esp::TeamPlayerNameRainbow
			)
			rainbow_color();

		if (LI_FN(GetAsyncKeyState)(VK_INSERT))
		{
			Variables::bMenu = !Variables::bMenu;
			Variables::WindowStatus = !Variables::WindowStatus;
			Sleep(150);
		}

		if (Variables::bMenu && Variables::loggedin) 
		{
			Menu();
			LocalImgui::ChangeClickAbility(true, MenuWindowHandle);
		}
		else if (Variables::bMenu && !Variables::loggedin)
		{
			Login();
			LocalImgui::ChangeClickAbility(true, MenuWindowHandle);
		}
		else
		{
			LocalImgui::ChangeClickAbility(false, MenuWindowHandle);
		}
		if (!Variables::WindowStatus) 
		{
			ShowWindow(MenuWindowHandle, SWP_SHOWWINDOW);
		}

		Renderer::GetInstance()->BeginScene();

		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, reinterpret_cast<const float*>(&Variables::clearColor));

		BoxEsp(RainbowSix::Entitys);

		Renderer::GetInstance()->DrawScene();
		Renderer::GetInstance()->EndScene();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		g_pSwapChain->Present(0, 0);
		SetWindowPos(MenuWindowHandle, Variables::PrevWindow, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
		UpdateWindow(MenuWindowHandle);
		
		return 0;
	}
}