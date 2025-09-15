#pragma once
#include "MonState_Daniela.h"

NS_BEGIN(Client)
class CDie_Daniela final : public CMonState_Daniela
{
private:
	CDie_Daniela();
	virtual ~CDie_Daniela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CDaniela* pContainer) override;
	virtual void				Update(CDaniela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CDaniela* pContainer) override;

private:
	_bool						m_isNoies = { false };
public:
	static CDie_Daniela*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END