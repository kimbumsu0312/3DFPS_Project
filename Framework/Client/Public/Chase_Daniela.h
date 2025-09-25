#pragma once
#include "MonState_Daniela.h"

NS_BEGIN(Client)
class CChase_Daniela final : public CMonState_Daniela
{
private:
	CChase_Daniela();
	virtual ~CChase_Daniela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CDaniela* pContainer) override;
	virtual void				Update(CDaniela* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CDaniela* pContainer) override;

private:
	_bool						m_bIsLight = {};
	_float						m_fAccTime = {};
public:
	static CChase_Daniela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END