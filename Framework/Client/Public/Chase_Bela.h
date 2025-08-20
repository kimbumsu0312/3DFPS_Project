#pragma once
#include "MonState_Bela.h"

NS_BEGIN(Client)
class CChase_Bela final : public CMonState_Bela
{
private:
	CChase_Bela();
	virtual ~CChase_Bela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CBela* pContainer) override;
	virtual void				Update(CBela* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CBela* pContainer) override;

public:
	static CChase_Bela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END