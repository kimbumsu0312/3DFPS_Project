#pragma once
#include "MonState_Bela.h"

NS_BEGIN(Client)
class CIdle_Bela final : public CMonState_Bela
{
private:
	CIdle_Bela();
	virtual ~CIdle_Bela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CBela* pContainer) override;
	virtual void				Update(CBela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CBela* pContainer) override;

public:
	static CIdle_Bela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END