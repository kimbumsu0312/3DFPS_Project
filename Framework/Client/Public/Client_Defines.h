#pragma once
#include "framework.h"
#include <process.h>
#include "Client_Item.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1920;
	const unsigned int			g_iWinSizeY = 1080;

	enum class SOUND_CHANNEL { BGM, PLAYER, PLAYER_MOVE, MONSTER, DANIELA, BELA, ALCINA, MAP, UI, END };
	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, END };
	enum class CAMERA_STATE { ZOOM_IN, ZOOM_OUT, ZOOM_RESET, END };
	enum class ITEM_2DRENDER_TYPE { HUD, INVEN, END };
}

#pragma comment(lib, "ole32.lib")
extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern float g_fBGMVolume;
using namespace Client;