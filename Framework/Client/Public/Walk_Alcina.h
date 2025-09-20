#pragma once
#include "MonState_Alcina.h"

NS_BEGIN(Client)
class CWalk_Alcina final : public CMonState_Alcina
{
private:
	CWalk_Alcina();
	virtual ~CWalk_Alcina() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CAlcina* pContainer) override;
	virtual void				Update(CAlcina* pContainer, _float fDeltatime) override;
	virtual void				Exit(CAlcina* pContainer) override;

private:
	_int						m_iPreTargetIndex = {};

	_bool						m_bIsLight = {};
public:
	static CWalk_Alcina*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END