#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_LightDesc;	}

	void			On_Light(_bool isOnOff) { m_isLight = isOnOff; }
	_bool			Get_LightOnOff() { return m_isLight; }
public:
	HRESULT			Initialize(const LIGHT_DESC& LightDesc);
	HRESULT			Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	void			Update_Postion(const _float4& vPostion);
private:
	LIGHT_DESC		m_LightDesc = {};
	_bool			m_isLight = {};
public:
	static CLight*	Create(const LIGHT_DESC& LightDesc);
	virtual void	Free();
};

NS_END