#pragma once
#include "MonState_Normal.h"
#include "Monstate_WereWolf.h"
NS_BEGIN(Client)
class CEvent_2_WereWolf final : public CMonState_WereWolf
{
private:
	enum class Event{TURN, WALK, END};
private:
	CEvent_2_WereWolf();
	virtual ~CEvent_2_WereWolf() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CMonster_WereWolf* pContainer) override;
	virtual void				Update(CMonster_WereWolf* pContainer, _float fDeltatime) override;
	virtual void				Exit(CMonster_WereWolf* pContainer) override;

private:
	Event						m_eEvent = { Event::END };
	_float						m_fMoveTime = 0.f;
	_int						m_iCount = 0;
public:
	static CEvent_2_WereWolf* Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
