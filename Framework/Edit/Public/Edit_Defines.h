#pragma once
#include "framework.h"
#include <process.h>


namespace Edit
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, MAP, MODEL, END };
	enum class CAMERA_STATE { ZOOM_IN, ZOOM_OUT, ZOOM_RESET, END };

	// Data
	//static IDXGISwapChain*			g_pSwapChain = nullptr;
	//static ID3D11RenderTargetView*	g_mainRenderTargetView = nullptr;
	//static bool						g_SwapChainOccluded = false;
	//static UINT						g_ResizeWidth = 0, g_ResizeHeight = 0;

}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
using namespace Edit;
using namespace ImGui;

#include "Edit_Defines.h"
