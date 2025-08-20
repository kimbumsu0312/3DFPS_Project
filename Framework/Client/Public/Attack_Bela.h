#pragma once
#include "MonState_Bela.h"

NS_BEGIN(Client)
class CAttack_Bela final : public CMonState_Bela
{
private:
	CAttack_Bela();
	virtual ~CAttack_Bela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CBela* pContainer) override;
	virtual void				Update(CBela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CBela* pContainer) override;

public:
	static CAttack_Bela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END