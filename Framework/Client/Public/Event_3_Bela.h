#pragma once
#include "MonState_Bela.h"

NS_BEGIN(Client)
class CEvent_3_Bela final : public CMonState_Bela
{
private:
	CEvent_3_Bela();
	virtual ~CEvent_3_Bela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CBela* pContainer) override;
	virtual void				Update(CBela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CBela* pContainer) override;

private:
	_float						m_fAccTime = {};
	_float						m_fEffectTime = {};
public:
	static CEvent_3_Bela* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END