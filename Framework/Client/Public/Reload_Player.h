#pragma once
#include "PlayerState.h"

NS_BEGIN(Client)
class CReload_Player final : public CPlayerState
{
private:
	enum class RELOAD_TYPE{ BULLET, MAGAZINE, END};
private:
	CReload_Player();
	virtual ~CReload_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) override;
	virtual void				Exit() override;
private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;
	RELOAD_TYPE					m_eReload_Type = RELOAD_TYPE::END;

public:
	static CReload_Player*		Create(void* pArg);
	virtual void				Free() override;
};
NS_END
