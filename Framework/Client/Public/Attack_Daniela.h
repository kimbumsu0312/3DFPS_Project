#pragma once
#include "MonState_Daniela.h"

NS_BEGIN(Client)
class CAttack_Daniela final : public CMonState_Daniela
{
private:
	CAttack_Daniela();
	virtual ~CAttack_Daniela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CDaniela* pContainer) override;
	virtual void				Update(CDaniela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CDaniela* pContainer) override;

private:
	_int						m_iAttackType = {};
	_float						m_fAccTime = {};

	_int						m_iAttack_ID = {};

public:
	static CAttack_Daniela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END