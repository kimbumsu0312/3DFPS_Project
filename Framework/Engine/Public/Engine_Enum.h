#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class DATA_TYPE { TERRAIN, LEVEL, END };
	enum class MODELTYPE { NONANIM, ANIM};
	enum class D3DTS { VIEW, PROJ, END};
	enum class STATE { RIGHT, UP, LOOK, POSITION };
	enum class PROTOTYPE { GAMEOBJECT, COMPONENT };
	enum class RENDERGROUP { PRIORITY, NONBLEND, BLEND, UI, UI_EFFECT, LAST, END };
	enum class WINMODE { FULL, WIN, END };
	enum class RENDERSTATE { BLEND, DEPTH_STENCIL, END };
	enum class OBJECTTYPE { GAMEOBJECT, POLLINGOBJECT, END};
	enum class MOUSEKEYSTATE { LB, RB, WB, END };
	enum class MOUSEMOVESTATE { X, Y, WHEEL, END };
}

#endif // Engine_Enum_h__
