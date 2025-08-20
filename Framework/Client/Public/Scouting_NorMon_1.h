#pragma once
#include "MonState_Normal.h"

NS_BEGIN(Client)
class CScouting_NorMon_1 final : public CMonState_Normal
{
private:
	enum class RELOAD_TYPE { BULLET, MAGAZINE, END };
private:
	CScouting_NorMon_1();
	virtual ~CScouting_NorMon_1() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CMonster_Normal* pContainer) override;
	virtual void				Update(CMonster_Normal* pContainer, _float fDeltatime) override;
	virtual void				Exit(CMonster_Normal* pContainer) override;

private:
	RELOAD_TYPE					m_eReload_Type = RELOAD_TYPE::END;

public:
	static CScouting_NorMon_1*	Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
