#pragma once
#include "MonState_Alcina.h"

NS_BEGIN(Client)
class CAttack1_Alcina final : public CMonState_Alcina
{
private:
	CAttack1_Alcina();
	virtual ~CAttack1_Alcina() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CAlcina* pContainer) override;
	virtual void				Update(CAlcina* pContainer, _float fDeltatime) override;
	virtual void				Exit(CAlcina* pContainer) override;

private:
	_int						m_iAttackCount = { 0 };
	_wstring					m_szAttackName = {};

	_bool						m_bisRight = { false };
public:
	static CAttack1_Alcina* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END