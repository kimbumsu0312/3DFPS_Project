#pragma once
#include "Engine_Defines.h"

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
}