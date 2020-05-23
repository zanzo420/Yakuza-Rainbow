#pragma once
#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_internal.h"

#define DIRECTINPUT_VERSION 0x0800
#include <d3d11.h>
#include <dinput.h>
#include <tchar.h>
#include <Dwmapi.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <Windows.h>
#include <fstream>
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

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Stats
{
    int    uniform_triangle_pixels = 0;
    int    textured_triangle_pixels = 0;
    int    gradient_triangle_pixels = 0;
    int    font_pixels = 0;
    double uniform_rectangle_pixels = 0;
    double textured_rectangle_pixels = 0;
    double gradient_rectangle_pixels = 0;
    double gradient_textured_rectangle_pixels = 0;
};
