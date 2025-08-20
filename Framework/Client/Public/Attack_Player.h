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
	virtual void				Enter(CPlayer* pContainer) override;
	virtual void				Update(CPlayer* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CPlayer* pContainer) override;

private:
	_bool						m_bMotionSwap= true;

public:
	static CAttack_Player*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
