#include "pch.h"
#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext}
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype) : CComponent(Prototype)
		, m_pVB { Prototype.m_pVB }, m_pIB { Prototype.m_pIB }
		, m_iNumVertices { Prototype.m_iNumVertices }
		, m_iVertexStride { Prototype.m_iVertexStride }
		, m_iNumIndices { Prototype.m_iNumIndices }
		, m_iIndexStride { Prototype.m_iIndexStride }
		, m_iNumVertexBuffers { Prototype.m_iNumVertexBuffers }
		, m_eIndexFormat { Prototype.m_eIndexFormat}
		, m_ePrimitiveType { Prototype.m_ePrimitiveType }
		, m_pVertexPositions{ Prototype.m_pVertexPositions }
		, m_pIndices {Prototype.m_pIndices}
		, m_pVertexTex(Prototype.m_pVertexTex)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Bind_Resources()
{
	ID3D11Buffer*	pVertexBuffers[] = { m_pVB, };
	_uint			iVertexStrides[] = { m_iVertexStride, };
	_uint			iOffsets[]		 = { 0, };

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

_bool CVIBuffer::IsPicked(CTransform& pTransform, _float3& pOut)
{
	_float3	vLocalPickPos = { pOut };

	_vector vWolrdPickPos = {};
	_bool	IsPicked = false;

	if (D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST != m_ePrimitiveType)
		return false;

	m_pGameInstance->TransformToLocalSpace(pTransform);
	_uint* pIndices = reinterpret_cast<_uint*>(m_pIndices);

	for (_int i = 0; i < m_iNumIndices / 3; i++)
	{
		_uint i0 = pIndices[i * 3 + 0];
		_uint i1 = pIndices[i * 3 + 1];
		_uint i2 = pIndices[i * 3 + 2];

		IsPicked = m_pGameInstance->isPickedInLocalSpace(m_pVertexPositions[i0], m_pVertexPositions[i1], m_pVertexPositions[i2], vLocalPickPos);
		if (IsPicked)
		{
			XMVECTOR vWorldPos = XMVector3TransformCoord(XMLoadFloat3(&vLocalPickPos), pTransform.Get_WorldMatrix());
			XMStoreFloat3(&pOut, vWorldPos);
			break;
		}
	
	}

	return IsPicked;
}

void CVIBuffer::Free()
{
	__super::Free();
	
	Safe_Delete_Array(m_pIndices);
	Safe_Delete_Array(m_pVertexPositions);
	Safe_Delete_Array(m_pVertexTex);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
