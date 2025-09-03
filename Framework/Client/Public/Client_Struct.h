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

	//스테이터스 관련 구조체
	typedef struct PlayerMoveState
	{
		bool isMoveF;
		bool isMoveB;
		bool isMoveR;
		bool isMoveL;
		bool isJog;
	}PLAYER_MOVE_STATE;

	typedef struct PlayerAttackState
	{
		bool isMove;
		bool isAttack;
		bool isGuard;
		bool isReload;
		bool isAim;
		bool isWeaponSwap;
	}PLAYER_ATTACK_STATE;

	typedef struct NorMonState
	{
		int  iWeponType;
		bool isChase;
		bool isAttack;
		bool isDamage;
		bool isDie;
	}NORMON_STATE;

	typedef struct Boss_SisterState
	{
		bool isChase;
		bool isAttack;
		bool isDamage;
		bool isDie;
	}BOSS_SISTER_STATE;

	typedef struct Boss_SisterHitPoint
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
		_wstring	szState;
		_int		iStartMotion;
	}POOLMONDESC;

}
