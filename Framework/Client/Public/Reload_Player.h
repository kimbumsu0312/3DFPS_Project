#pragma once
#include "StateObject.h"

NS_BEGIN(Client)
class CReload_Player final : public CStateObject
{
private:
	enum class RELOAD_TYPE{ BULLET, MAGAZINE, END};
private:
	CReload_Player();
	virtual ~CReload_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter() override;
	virtual void				Update(_float fDeltatime) override;
	virtual void				Exit() override;

private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;
	RELOAD_TYPE					m_eReload_Type = RELOAD_TYPE::END;
private:
	void						KeyInput();
public:
	static CReload_Player* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
