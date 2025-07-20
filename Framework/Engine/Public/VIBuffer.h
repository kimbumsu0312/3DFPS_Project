#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

public:
	HRESULT					Bind_Resources();
	HRESULT					Render();
	_bool					IsPicked(class CTransform& pTransform, _float3& pOut);

protected:
	ID3D11Buffer*			m_pVB = { nullptr };
	ID3D11Buffer*			m_pIB = { nullptr };
	
	_uint					m_iNumVertices = {};
	_uint					m_iVertexStride = {};
	_uint					m_iNumIndices = {};
	_uint					m_iIndexStride = {};
	_uint					m_iNumVertexBuffers = {};
	DXGI_FORMAT				m_eIndexFormat = {};
	D3D_PRIMITIVE_TOPOLOGY	m_ePrimitiveType = {};

	_float3*				m_pVertexPositions = { nullptr };
	_float2*				m_pVertexTex = { nullptr };
	_uint*					m_pIndices = { nullptr };
public:
	virtual CComponent*		Clone(void* pArg) = 0;
	virtual void			Free();
};

NS_END