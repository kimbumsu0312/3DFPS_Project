#pragma once
#include "MonState_Normal.h"

NS_BEGIN(Client)
class CIdle_NorMon_1 final : public CMonState_Normal
{
private:
	enum class IDLE_TYPE {STAND, SIT, CLIME};

private:
	CIdle_NorMon_1();
	virtual ~CIdle_NorMon_1() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CMonster_Normal* pContainer) override;
	virtual void				Update(CMonster_Normal* pContainer, _float fDeltatime) override;
	virtual void				Exit(CMonster_Normal* pContainer) override;

private:
	_int						m_iStartType = 0;
public:
	static CIdle_NorMon_1*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
