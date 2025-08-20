#pragma once
#include "MonState_Daniela.h"

NS_BEGIN(Client)
class CIdle_Daniela final : public CMonState_Daniela
{
private:
	CIdle_Daniela();
	virtual ~CIdle_Daniela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CDaniela* pContainer) override;
	virtual void				Update(CDaniela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CDaniela* pContainer) override;

public:
	static CIdle_Daniela*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END