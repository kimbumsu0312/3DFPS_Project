#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC*		Get_LightDesc(_uint iIndex);
	HRESULT					Add_Light(LIGHT_DESC& LightDesc);

private:
	list<class CLight*>		m_Lights;

public:
	static CLight_Manager*	Create();
	virtual void			Free() override;
};

NS_END