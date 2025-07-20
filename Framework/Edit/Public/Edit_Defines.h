#pragma once
#include "framework.h"
#include <process.h>

//Imgui
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
//매쉬 파일 로드 헤더
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

namespace Edit
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, MAP, MODEL, END };
	enum class CAMERA_STATE { ZOOM_IN, ZOOM_OUT, ZOOM_RESET, END };
}
extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern bool g_CreateModel;
extern bool g_SeleteModel;
extern bool g_MoveModel;
extern bool g_TerrainHight;

using namespace Edit;
using namespace ImGui;

#include "Edit_Defines.h"
