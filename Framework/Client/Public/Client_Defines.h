#pragma once
#include "framework.h"
#include <process.h>
#include <Client_Event.h>

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
using namespace Client;