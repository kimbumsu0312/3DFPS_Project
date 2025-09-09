#pragma once

namespace Client
{
	typedef struct tagRECT
	{
		_int    left;
		_int    top;
		_int    right;
		_int    bottom;
	}INVENRECT;

	typedef struct InvenItem {
		_int iItemIndex;
		_int iInvenSizeX;
		_int iInvenSizeY;
		_int iItemGridX;
		_int iItemGridY;

		_int iItemCount;
	}INVEN_ITEM;

	typedef struct BOSS_SISTERHitPoint
	{
		bool IsHead;
		bool IsBody;
		bool IsSholder_L;
		bool isSholder_R;
	}BOSS_SISTER_HIT;

	//풀링 관련 구조체
	typedef struct PoolMonDesc {
		_int		iAnimState;
		string		szAnimTag;
		_int		iWeponType;
		_int		iCellIndex;
		_vector		vPostion;
		_float		vAngleY;
		_wstring	szState;
		_int		iStartMotion;
		_int        iDropImteIndex;
	}POOLMONDESC;

}
