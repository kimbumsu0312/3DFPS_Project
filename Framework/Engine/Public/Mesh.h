#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	virtual HRESULT			Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual HRESULT			Initialize(void* pArg) override;
	_uint					Get_MaterialIndex() { return m_iMaterialIndex; }

private:
	_uint					m_iMaterialIndex = {};

public:
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END
