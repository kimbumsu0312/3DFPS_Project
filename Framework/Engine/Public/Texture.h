#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	virtual HRESULT								Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT								Initialize(void* pArg) override;

public:
	HRESULT										Bind_Shader_Resource(class CShader* pShader, const _char* pConstantName, _uint iTextureIndex);
	HRESULT										Bind_Shader_Resource_IndexCheck(class CShader* pShader, const _char* pConstantName, _uint iTextureIndex);

private:
	//�ؽ�ó 
	vector<ID3D11ShaderResourceView*>			m_SRVs;
	_uint										m_iNumTextures  = { };
	_uint										m_iLastTexIndex = {};

public:
	static CTexture*							Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent*							Clone(void* pArg) override;
	virtual void								Free() override;

};

NS_END