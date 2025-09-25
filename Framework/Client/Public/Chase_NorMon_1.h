#pragma once
#include "MonState_Normal.h"

NS_BEGIN(Client)
class CChase_NorMon_1 final : public CMonState_Normal
{

private:
	CChase_NorMon_1();
	virtual ~CChase_NorMon_1() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CMonster_Normal* pContainer) override;
	virtual void				Update(CMonster_Normal* pContainer, _float fDeltatime) override;
	virtual void				Exit(CMonster_Normal* pContainer) override;

private:
	_int						m_iWeapon{};
	_float						m_fAccTime{};
public:
	static CChase_NorMon_1*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
