#pragma once
#include "StateObject.h"

NS_BEGIN(Client)
class CGuard_Player final : public CStateObject
{
private:
	CGuard_Player();
	virtual ~CGuard_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter() override;
	virtual void				Update(_float fDeltatime) override;
	virtual void				Exit() override;

private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;

private:
	void						KeyInput();
public:
	static CGuard_Player* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
