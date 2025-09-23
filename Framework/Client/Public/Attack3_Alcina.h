#pragma once
#include "MonState_Alcina.h"

NS_BEGIN(Client)
class CAttack3_Alcina final : public CMonState_Alcina
{
private:
	CAttack3_Alcina();
	virtual ~CAttack3_Alcina() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CAlcina* pContainer) override;
	virtual void				Update(CAlcina* pContainer, _float fDeltatime) override;
	virtual void				Exit(CAlcina* pContainer) override;

private:
	_int						m_iAttackCount = { 0 };
	_wstring					m_szAttackName = {};

public:
	static CAttack3_Alcina*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END