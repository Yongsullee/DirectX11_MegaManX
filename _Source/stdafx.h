#pragma once

#include <windows.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <random>
#include <iomanip>
using namespace std;

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")


//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

#include "Renders/Shader.h"
#include "Renders/Sprite.h"
#include "Renders/Clip.h"
#include "Renders/Animation.h"

#include "Systems/Keyboard.h"
#include "Systems/CMouse.h"
#include "Systems/Time.h"
#include "Systems/DirectWrite.h"

#include "Utilities/Math.h"
#include "Utilities/Path.h"
#include "Utilities/String.h"

#define SAFE_RELEASE(p) { if(p){(p)->Release(); (p) = NULL;}}
#define SAFE_DELETE(p) {if(p){delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p){if(p) {delete [] (p); (p) = NULL; } }


const wstring Textures = L"../_Textures/";
const wstring Shaders = L"../_Shaders/";

extern HWND Hwnd;
extern wstring Title;

extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

extern UINT Width;
extern UINT Height;

extern Keyboard* Key;
extern Time* Timer;
extern CMouse* Mouse;
