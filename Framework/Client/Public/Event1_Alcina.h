#pragma once
#include "MonState_Alcina.h"

NS_BEGIN(Client)
class CEvent1_Alcina final : public CMonState_Alcina
{
private:
	CEvent1_Alcina();
	virtual ~CEvent1_Alcina() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CAlcina* pContainer) override;
	virtual void				Update(CAlcina* pContainer, _float fDeltatime) override;
	virtual void				Exit(CAlcina* pContainer) override;

public:
	static CEvent1_Alcina* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END