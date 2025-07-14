#pragma once
#include "pch.h"
#include "Engine_Defines.h"
#include "Client_Item.h"

namespace Client
{
	typedef struct Next_Level {
	}Event_NextLevel;

	typedef struct NonSelete_LogoButton {
	}Event_NonSelete_LogoButton;

	typedef struct Selete_LogoButton {
		_uint iIndex;
	}Event_Selete_LogoButton;

	typedef struct Selete_LogoButton_Index {
		_uint iIndex;
	}Event_Selete_LogoButton_Index;

	typedef struct Level_Change {
	}Event_Level_Change;

	typedef struct Inventory_Open {
		_bool bIsOpen;
	}Event_Inventory_Open;

	typedef struct Hud_Gun_Zoom_Aim {
		_bool bIsAIm;
	}Event_HUD_GUN_AIM;

	typedef struct Hud_Gun_Zoom {
		_bool bZoom;
	}Event_HUD_GUN;

	typedef struct Hud_Weapon_Selete {
		WEAPON_TYPE eType;
	}Event_Weapon_Selete;

	typedef struct Camera_Zoom {
		CAMERA_STATE eState;
		_float fZoomSpeed;
		_float fZoomFov;
	}Event_Camera_Zoom;

	typedef struct Hud_Weapon_Shoting {
	}Event_Weapon_Shoting;

	typedef struct Player_Hp_Set
	{
		_float4 Color;
	}Event_Player_Hp_Set;

	typedef struct Player_Hp_UI_Open
	{
		_uint itype;
	}Event_Player_Hp_UI_Open;

	typedef struct Inven_Selete_Penal
	{
		_int iIndex;
	}Event_Inven_Selete_penal;
}