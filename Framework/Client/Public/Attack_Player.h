#pragma once
#include "StateObject.h"

NS_BEGIN(Client)
class CAttack_Player final : public CStateObject
{

private:
	CAttack_Player();
	virtual ~CAttack_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter() override;
	virtual void				Update(_float fDeltatime) override;
	virtual void				Exit() override;

private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;
	_bool						m_bAttackPatten = true;

public:
	static CAttack_Player*		Create(void* pArg);
	virtual void				Free() override;
};
NS_END
