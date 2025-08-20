#pragma once
#include "MonState_Daniela.h"

NS_BEGIN(Client)
class CDamage_Daniela final : public CMonState_Daniela
{
private:
	CDamage_Daniela();
	virtual ~CDamage_Daniela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CDaniela* pContainer) override;
	virtual void				Update(CDaniela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CDaniela* pContainer) override;

private:
	void						Normal_Damage(CDaniela* pContainer);
	void						Big_Damage(CDaniela* pContainer);

public:
	static CDamage_Daniela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END