#pragma once
#include "Edit_Defines.h"

namespace Edit
{
	typedef struct Next_Level {
		LEVEL eLevel;
	}Event_NextLevel;

	typedef struct Clear_Map {
	}Clear_Map;

	typedef struct Create_Modle {
	}Create_Modle;

	typedef struct Erase_Model {
	}Event_Erase_Model;
	
	typedef struct Model_Index_Set {
		_int i;
	}Event_Model_Index_Set;


}