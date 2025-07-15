#pragma once
#include "framework.h"
#include <process.h>

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