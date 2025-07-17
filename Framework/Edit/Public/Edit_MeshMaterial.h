#pragma once
#include "Base.h"
#include "Engine_Defines.h"

NS_BEGIN(Edit)
class CEdit_MeshMaterial final : public CBase
{
private:
	CEdit_MeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEdit_MeshMaterial() = default;

public:
	HRESULT								Initialize(const _char* pModelFilePath, const aiMaterial* pAIMaterial);
	HRESULT								Bind_Shader_Resource(CShader* pShader, const _char* pConstantName, aiTextureType eTextureType, _uint iIndex);

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>	m_SRVs[AI_TEXTURE_TYPE_MAX];

public:
	static CEdit_MeshMaterial*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const aiMaterial* pAIMaterial);
	virtual void						Free() override;

};
NS_END
