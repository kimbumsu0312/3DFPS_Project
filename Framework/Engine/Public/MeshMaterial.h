#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CMeshMaterial final : public CBase
{
private:
	CMeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMeshMaterial() = default;

public:
	HRESULT										Initialize(const SAVE_MESHMATERIAL& pMaterialData);
	HRESULT										Bind_Resources(class CShader* pShader, const _char* pConstantName, _int iTexIndex, _uint iIndex);
private:
	ID3D11Device*								m_pDevice = { nullptr };
	ID3D11DeviceContext*						m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>			m_SRVs[26];

public:
	static CMeshMaterial*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const SAVE_MESHMATERIAL& pMaterialData);
	virtual void								Free() override;
};

NS_END
