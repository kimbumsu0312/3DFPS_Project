#pragma once
#include "MonState_Bela.h"

NS_BEGIN(Client)
class CWalk_Bela final : public CMonState_Bela
{
private:
	CWalk_Bela();
	virtual ~CWalk_Bela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CBela* pContainer) override;
	virtual void				Update(CBela* pContainer, _float fTimeDelta) override;
	virtual void				Exit(CBela* pContainer) override;

private:
	_float						m_fAccTime = {};
	_float						m_fSpeed = {};
public:
	static CWalk_Bela*			Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END