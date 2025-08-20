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
	virtual void				Enter(CPlayer* pContainer) override;
	virtual void				Update(CPlayer* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CPlayer* pContainer) override;

	RELOAD_TYPE					m_eReload_Type = RELOAD_TYPE::END;

public:
	static CReload_Player*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
