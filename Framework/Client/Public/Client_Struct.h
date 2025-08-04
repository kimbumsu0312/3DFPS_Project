#pragma once

namespace Client
{
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
		bool isAttack;
		bool isGuard;
		bool isReload;
		bool isAim;
	}PLAYER_ATTACK_STATE;

	typedef struct NorMonState
	{
		int  iWeponType;
		bool isChase;
		bool isAttack;
		bool isDamage;
	}NORMON_STATE;
}
