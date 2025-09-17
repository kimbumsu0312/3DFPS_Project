#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC*					Get_LightDesc(_wstring LightTag);
	HRESULT								Add_Light(_wstring LightTag, LIGHT_DESC& LightDesc);
	HRESULT								Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	_bool								Update_LightPotion(_wstring LightTag, _float4 LightPos);
	_bool								OnOff_Light(_wstring LightTag, _bool isOnoff);
private:
	unordered_map<_wstring,
		class CLight*>					m_Lights;

private:
	CLight*								Find_Light(_wstring LightTag);
public:
	static CLight_Manager*				Create();
	virtual void						Free() override;
};

NS_END