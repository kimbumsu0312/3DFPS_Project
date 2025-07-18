#pragma once
#include "VIBuffer.h"

NS_BEGIN(Edit)
class CEdit_Mesh final : public CVIBuffer
{
private:
	CEdit_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEdit_Mesh(const CEdit_Mesh& Prototype);
	virtual ~CEdit_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;

	_uint			Get_MaterialIndex() { return m_iMaterialIndex; }

private:
	_uint			m_iMaterialIndex = {};

public:
	static CEdit_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END