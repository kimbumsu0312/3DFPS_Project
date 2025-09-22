#include "pch.h"
#include "VIBuffer_Mesh_Instance.h"

CVIBuffer_Mesh_Instance::CVIBuffer_Mesh_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CVIBuffer_Instance{ pDevice, pContext }
{
}

CVIBuffer_Mesh_Instance::CVIBuffer_Mesh_Instance(const CVIBuffer_Mesh_Instance& Prototype) : CVIBuffer_Instance(Prototype)
, m_vPivot{ Prototype.m_vPivot }
, m_pSpeeds{ Prototype.m_pSpeeds }
, m_isLoop{ Prototype.m_isLoop }
{
}

HRESULT CVIBuffer_Mesh_Instance::Initialize_Prototype(const INSTANCE_DESC* pDesc)
{
	const MESH_INSTANCE_DESC* pMeshDesc = static_cast<const MESH_INSTANCE_DESC*>(pDesc);

	m_vPivot = pMeshDesc->vPivot;
	m_isLoop = pMeshDesc->isLoop;

	m_iInstanceVertexStride = sizeof(VTXINSTANCE_MESH);
	m_iNumInstance = pMeshDesc->iNumInstance;
	m_iNumVertexBuffers = 1;

	m_VBInstanceDesc.ByteWidth = m_iNumInstance * m_iInstanceVertexStride;
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.MiscFlags = 0;
	m_VBInstanceDesc.StructureByteStride = m_iInstanceVertexStride;

	m_pInstanceVertices = new VTXINSTANCE_MESH[m_iNumInstance];
	m_pSpeeds = new _float[m_iNumInstance];

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		VTXINSTANCE_MESH* pInstanceVertices = static_cast<VTXINSTANCE_MESH*>(m_pInstanceVertices);
		pMeshDesc->vPivot;

		_float fScale = pInstanceVertices->fScale = m_pGameInstance->Rand(pMeshDesc->vSize.x, pMeshDesc->vSize.y);
		_float		fLifeTime = m_pGameInstance->Rand(pMeshDesc->vLifeTime.x, pMeshDesc->vLifeTime.y);
		m_pSpeeds[i] = m_pGameInstance->Rand(pMeshDesc->vSpeed.x, pMeshDesc->vSpeed.y);

		pInstanceVertices[i].vTranslation = _float4(
			m_pGameInstance->Rand(pMeshDesc->vCenter.x - pMeshDesc->vRange.x * 0.5f, pMeshDesc->vCenter.x + pMeshDesc->vRange.x * 0.5f),
			m_pGameInstance->Rand(pMeshDesc->vCenter.y - pMeshDesc->vRange.y * 0.5f, pMeshDesc->vCenter.y + pMeshDesc->vRange.y * 0.5f),
			m_pGameInstance->Rand(pMeshDesc->vCenter.z - pMeshDesc->vRange.z * 0.5f, pMeshDesc->vCenter.z + pMeshDesc->vRange.z * 0.5f),
			1.f
		);

		_vector vRight = XMVector3Normalize(XMVectorSetW(-XMLoadFloat4(&pInstanceVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f));
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };
		_vector vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));;
		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

		XMStoreFloat4(&pInstanceVertices[i].vRight, XMVectorSetW(XMVectorScale(vRight, fScale), 0.f));
		XMStoreFloat4(&pInstanceVertices[i].vUp, XMVectorSetW(XMVectorScale(vUp, fScale), 0.f));
		XMStoreFloat4(&pInstanceVertices[i].vLook, XMVectorSetW(XMVectorScale(vLook, fScale), 0.f));

		pInstanceVertices[i].vLifeTime = _float2(0.f, fLifeTime);


	}

	return S_OK;
}

HRESULT CVIBuffer_Mesh_Instance::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Mesh_Instance::Bind_Resources()
{
	ID3D11Buffer* pVertexBuffers[] = { m_pVBInstance };
	_uint iVertexStrides[] = { m_iInstanceVertexStride };
	_uint iOffSets[] = { 0 };

	m_pContext->IASetVertexBuffers(1, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffSets);

	return S_OK;
}

void CVIBuffer_Mesh_Instance::Spread(_float fTimeDelta, _bool isLife)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	VTXINSTANCE_MESH* pInstanceVertices = static_cast<VTXINSTANCE_MESH*>(m_pInstanceVertices);

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE_MESH* pVertices = static_cast<VTXINSTANCE_MESH*>(SubResource.pData);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_vector	vMoveDir = XMVector3Normalize(XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f));

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + vMoveDir * m_pSpeeds[i] * fTimeDelta);
		
		if (!isLife)
			continue;

		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop)
		{
			if (pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
			{
				pVertices[i].vLifeTime.x = 0.f;
				pVertices[i].vTranslation = pInstanceVertices[i].vTranslation;
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Mesh_Instance::LocalOffset_Spin(_float fTimeDelta, _bool isLife)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	VTXINSTANCE_MESH* pInstanceVertices = static_cast<VTXINSTANCE_MESH*>(m_pInstanceVertices);

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE_MESH* pVertices = static_cast<VTXINSTANCE_MESH*>(SubResource.pData);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		//룩 방향 이동
		_vector vMoveDir = XMVector3Normalize(XMLoadFloat4(&pVertices[i].vLook));
		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + vMoveDir * m_pSpeeds[i] * fTimeDelta);
		
		//피봇 기준 회전
		_vector vRight = XMVector3Normalize(XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f));
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };
		_vector vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));;
		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

		XMStoreFloat4(&pVertices[i].vRight, XMVectorSetW(XMVectorScale(vRight, pInstanceVertices->fScale), 0.f));
		XMStoreFloat4(&pVertices[i].vUp, XMVectorSetW(XMVectorScale(vUp, pInstanceVertices->fScale), 0.f));
		XMStoreFloat4(&pVertices[i].vLook, XMVectorSetW(XMVectorScale(vLook, pInstanceVertices->fScale), 0.f));

		if (!isLife)
			continue;

		pVertices[i].vLifeTime.x += fTimeDelta;
		if (true == m_isLoop)
		{
			if (pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
			{
				pVertices[i].vLifeTime.x = 0.f;
				pVertices[i].vTranslation = pInstanceVertices[i].vTranslation;
			}
		}

	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Mesh_Instance::Pivot_Spin(_float fTimeDelta, _float3 vOffset)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	VTXINSTANCE_MESH* pInstanceVertices = static_cast<VTXINSTANCE_MESH*>(m_pInstanceVertices);

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE_MESH* pVertices = static_cast<VTXINSTANCE_MESH*>(SubResource.pData);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_vector	vMoveDir = XMVector3Normalize(XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f));

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + vMoveDir * m_pSpeeds[i] * fTimeDelta);
		pVertices[i].vLifeTime.x += fTimeDelta;
	
		_vector vRight = XMVector3Normalize(XMVectorSetW(XMLoadFloat3(&m_vPivot) - XMLoadFloat4(&pVertices[i].vTranslation), 0.f));
		_vector vUp = XMLoadFloat4(&pVertices->vUp);
		_vector vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));;
		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

		_vector Offset = XMVector3Rotate(XMLoadFloat3(&vOffset), vRight);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pInstanceVertices[i].vTranslation) + Offset);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Mesh_Instance::Reset()
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	VTXINSTANCE_PARTICLE* pInstanceVertices = static_cast<VTXINSTANCE_PARTICLE*>(m_pInstanceVertices);

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE_PARTICLE* pVertices = static_cast<VTXINSTANCE_PARTICLE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.x = 0.f;
		pVertices[i].vTranslation = pInstanceVertices[i].vTranslation;

	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Mesh_Instance::WorldOffset_Spin(_float fTimeDelta, _float3 vOffset)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	VTXINSTANCE_MESH* pInstanceVertices = static_cast<VTXINSTANCE_MESH*>(m_pInstanceVertices);

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE_MESH* pVertices = static_cast<VTXINSTANCE_MESH*>(SubResource.pData);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_vector vLook = XMLoadFloat4(&pVertices[i].vLook);
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };
		_vector vRight = XMVector3Normalize(XMVector3Cross(vLook, vUp));

		_vector Offset = XMVector3Rotate(XMLoadFloat3(&vOffset), vRight);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pInstanceVertices[i].vTranslation) + Offset);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Mesh_Instance* CVIBuffer_Mesh_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pDesc)
{
	CVIBuffer_Mesh_Instance* pInstance = new CVIBuffer_Mesh_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Mesh_Instance"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Mesh_Instance::Clone(void* pArg)
{
	CVIBuffer_Mesh_Instance* pInstance = new CVIBuffer_Mesh_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Mesh_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Mesh_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pSpeeds);
}
