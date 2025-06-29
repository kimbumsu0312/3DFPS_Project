#pragma once
#include "framework.h"
#include <process.h>

/* Ŭ���̾�Ʈ���� ����� �� �ִ� �������� ���Ǹ� ��Ƴ��� ���� */
namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
using namespace Client;