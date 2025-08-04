#pragma once
#include "PlayerState.h"

NS_BEGIN(Client)
class CGuard_Player final : public CPlayerState
{
private:
	CGuard_Player();
	virtual ~CGuard_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Exit() override;

private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;

public:
	static CGuard_Player* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
