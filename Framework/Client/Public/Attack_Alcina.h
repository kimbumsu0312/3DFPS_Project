#pragma once
#include "MonState_Alcina.h"

NS_BEGIN(Client)
class CAttack_Alcina final : public CMonState_Alcina
{
private:
	CAttack_Alcina();
	virtual ~CAttack_Alcina() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CAlcina* pContainer) override;
	virtual void				Update(CAlcina* pContainer, _float fDeltatime) override;
	virtual void				Exit(CAlcina* pContainer) override;

public:
	static CAttack_Alcina* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END