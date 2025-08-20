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
	virtual void				Enter(CPlayer* pContainer) override;
	virtual void				Update(CPlayer* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CPlayer* pContainer) override;

public:
	static CGuard_Player*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
