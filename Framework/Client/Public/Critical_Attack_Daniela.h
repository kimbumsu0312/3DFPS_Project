#pragma once
#include "MonState_Daniela.h"

NS_BEGIN(Client)
class CCritical_Attack_Daniela final : public CMonState_Daniela
{
private:
	CCritical_Attack_Daniela();
	virtual ~CCritical_Attack_Daniela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CDaniela* pContainer) override;
	virtual void				Update(CDaniela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CDaniela* pContainer) override;

public:
	static CCritical_Attack_Daniela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

class Critical_Attack_Daniela
{
};

NS_END