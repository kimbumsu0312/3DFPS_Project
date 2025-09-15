#pragma once
#include "MonState_Bela.h"

NS_BEGIN(Client)
class CDie_Bela final : public CMonState_Bela
{
private:
	CDie_Bela();
	virtual ~CDie_Bela() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CBela* pContainer) override;
	virtual void				Update(CBela* pContainer, _float fDeltatime) override;
	virtual void				Exit(CBela* pContainer) override;

private:
	_bool						m_isNoies = { false };
public:
	static CDie_Bela*			Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END