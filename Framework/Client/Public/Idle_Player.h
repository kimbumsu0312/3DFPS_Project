#pragma once
#include "PlayerState.h"

NS_BEGIN(Client)
class CIdle_Player final : public CPlayerState
{
private:
	CIdle_Player();
	virtual ~CIdle_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CPlayer* pContainer) override;
	virtual void				Update(CPlayer* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CPlayer* pContainer) override;

public:
	static CIdle_Player*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
