#pragma once
#include "PlayerState.h"

NS_BEGIN(Client)
class CAttack_Player final : public CPlayerState
{

private:
	CAttack_Player();
	virtual ~CAttack_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Exit() override;

private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;
	_bool						m_bAttackPatten = true;

public:
	static CAttack_Player*		Create(void* pArg);
	virtual void				Free() override;
};
NS_END
