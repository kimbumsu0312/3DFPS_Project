#pragma once
#include "PlayerState.h"

NS_BEGIN(Client)
class CWeaponSwap_Player final : public CPlayerState
{
private:
	CWeaponSwap_Player();
	virtual ~CWeaponSwap_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Exit() override;

private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;

public:
	static CWeaponSwap_Player* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
