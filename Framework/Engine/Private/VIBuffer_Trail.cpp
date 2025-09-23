#include "pch.h"
#include "VIBuffer_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& Prototype) : CVIBuffer(Prototype)
, m_fDeleyTime { Prototype.m_fDeleyTime}
, m_fAccTime { Prototype.m_fAccTime }
, m_iMaxNumVertices { Prototype.m_iMaxNumVertices }
, m_VBDesc {Prototype.m_VBDesc}
, m_pVertices{ Prototype.m_pVertices }

{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	m_iMaxNumVertices = 30;
	m_fDeleyTime = 0.05f;
	m_fAccTime = 0.1f;
	m_iNumVertices = 0;
	m_iVertexStride = sizeof(VTXPOSTEX_TRAIL);
	m_iNumVertexBuffers = 1;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_VBDesc.ByteWidth = m_iMaxNumVertices * m_iVertexStride;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iVertexStride;

	m_pVertices = new VTXPOSTEX_TRAIL[m_iMaxNumVertices];

	VTXPOSTEX_TRAIL* pVerticesData = static_cast<VTXPOSTEX_TRAIL*>(m_pVertices);

	for (_int i = 0; i < m_iMaxNumVertices; ++i)
	{
		pVerticesData[i].vPosition = _float3(0.f, 0.f, 0.f);
		pVerticesData[i].vLifeTime = _float2{ 0.f, 1.f };
	}

	return S_OK;
}


HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	D3D11_SUBRESOURCE_DATA InitialDesc{};
	InitialDesc.pSysMem = m_pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &InitialDesc, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Trail::Render()
{
	if(m_iNumVertices > 2)
		m_pContext->Draw(m_iNumVertices, 0);

	return S_OK;
}

HRESULT CVIBuffer_Trail::Bind_Resources()
{
	ID3D11Buffer* pVertexBuffers[] = { m_pVB };
	_uint iVertexStrides[] = { m_iVertexStride };
	_uint iOffsets[] = { 0, 0 };

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);

	return S_OK;
}

void CVIBuffer_Trail::Update_Trail(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOSTEX_TRAIL* pVertices = static_cast<VTXPOSTEX_TRAIL*>(SubResource.pData);

	for (_int i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vLifeTime.x += fTimeDelta;
	}

	m_pContext->Unmap(m_pVB, 0);
}

void CVIBuffer_Trail::Add_Postion(_float3 vTopPos, _float3 vBottomPos, _float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	if (m_fAccTime < m_fDeleyTime)
		return;
	m_fAccTime = 0.f;
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOSTEX_TRAIL* pVertices = static_cast<VTXPOSTEX_TRAIL*>(SubResource.pData);

	if (m_iMaxNumVertices > m_iNumVertices)
	{
		pVertices[m_iNumVertices].vPosition = vTopPos;
		pVertices[m_iNumVertices + 1].vPosition = vBottomPos;
		m_iNumVertices += 2;
	}
	else
	{
		memmove(&pVertices[0], &pVertices[2], sizeof(VTXPOSTEX_TRAIL) * (m_iMaxNumVertices - 2));
		pVertices[m_iMaxNumVertices - 2].vPosition = vTopPos;
		pVertices[m_iMaxNumVertices - 1].vPosition = vBottomPos;
	}

	if (m_iNumVertices > 2)
	{
		for (_int i = 0; i < m_iNumVertices; ++i)
		{
			_float v = (_float)i / (m_iNumVertices - 1);

			pVertices[i * 2 + 0].vTexcoord = _float2(0.f, v);
			pVertices[i * 2 + 1].vTexcoord = _float2(1.f, v);
		}
	}

	m_pContext->Unmap(m_pVB, 0);

}

void CVIBuffer_Trail::Reset()
{
	m_fAccTime = m_fDeleyTime;
	m_iNumVertices = 0;
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOSTEX_TRAIL* pVertices = static_cast<VTXPOSTEX_TRAIL*>(SubResource.pData);
	for (_int i = 0; i < m_iMaxNumVertices; ++i)
	{
		pVertices[i].vLifeTime.x = 0.f;
	}

	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Trail"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Trail"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertices);
}
