#pragma once
#include "framework.h"
#include <process.h>

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include "Edit_Defines.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

namespace Edit
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, MAP, MODEL, END };
	enum class CAMERA_STATE { ZOOM_IN, ZOOM_OUT, ZOOM_RESET, END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
using namespace Edit;

