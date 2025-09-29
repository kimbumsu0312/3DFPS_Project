#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CLut final : public CBase
{
private:
	CLut();
	virtual ~CLut() = default;

public:
	HRESULT				Set_TexPass(_int iPass);

public:
	HRESULT				Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT				Bind_LutTexture(class CShader* pShader, const _char* pConstantName);

private:
	class CTexture*		m_pTexture = { nullptr };
	_int				m_iTexPass = {};
	_int				m_iMaxPass = {};

public:
	static CLut*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual void		Free();
};

NS_END