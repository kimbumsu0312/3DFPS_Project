#include "pch.h"
#include "Model_Instance.h"
#include "VIBuffer_Mesh_Instance.h"
#include "Mesh.h"
#include "MeshMaterial.h"

CModel_Instance::CModel_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext }
{
}

CModel_Instance::CModel_Instance(const CModel_Instance& Prototype) : CComponent(Prototype)
, m_iNumMaterials (Prototype.m_iNumMaterials), m_iNumMeshes ( Prototype.m_iNumMeshes)
, m_Meshes(Prototype.m_Meshes)
, m_Materials(Prototype.m_Materials)
, m_eModelType(Prototype.m_eModelType)
, m_PreTransformMatrix(Prototype.m_PreTransformMatrix)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

	m_pInstance_Buffer = static_cast<CVIBuffer_Mesh_Instance*>(Prototype.m_pInstance_Buffer->Clone(nullptr));
}

HRESULT CModel_Instance::Initialize_Prototype(const SAVE_MODEL& pModelData, const CVIBuffer_Instance::INSTANCE_DESC* pInstance)
{
	m_eModelType = pModelData.eModel;
	m_PreTransformMatrix = pModelData.PreTransformMatrix;
	
	if (FAILED(Ready_Instance_Buffer(pInstance)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelData)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(pModelData)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel_Instance::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CModel_Instance::Bind_Instance()
{
	m_pInstance_Buffer->Bind_Resources();

	return S_OK;
}

HRESULT CModel_Instance::Bind_Materials(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, _int iTexIndex, _uint iIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint       iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (m_iNumMaterials <= iMaterialIndex)
		return E_FAIL;

	return m_Materials[iMaterialIndex]->Bind_Resources(pShader, pConstantName, iTexIndex, iIndex);
}

HRESULT CModel_Instance::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Resources();
	m_pContext->DrawIndexedInstanced(m_Meshes[iMeshIndex]->Get_NumIndices(), m_pInstance_Buffer->Get_NumInstance(), 0, 0, 0);
	
	return S_OK;
}

void CModel_Instance::Spread(_float fTimeDelta)
{
	m_pInstance_Buffer->Spread(fTimeDelta);
}

HRESULT CModel_Instance::Ready_Instance_Buffer(const CVIBuffer_Instance::INSTANCE_DESC* pDesc)
{
	CVIBuffer_Mesh_Instance* pMeshInstance = CVIBuffer_Mesh_Instance::Create(m_pDevice, m_pContext, pDesc);

	if (pMeshInstance == nullptr)
		return E_FAIL;

	m_pInstance_Buffer = pMeshInstance;

	return S_OK;
}

HRESULT CModel_Instance::Ready_Meshes(const SAVE_MODEL& pModelData)
{
	m_iNumMeshes = pModelData.iNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, pModelData.Meshs[i]);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel_Instance::Ready_Materials(const SAVE_MODEL& pModelData)
{

	m_iNumMaterials = pModelData.iNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{

		CMeshMaterial* pMeshMaterial = CMeshMaterial::Create(m_pDevice, m_pContext, pModelData.MeshMaterials[i]);
		if (nullptr == pMeshMaterial)
			return E_FAIL;

		m_Materials.push_back(pMeshMaterial);
	}

	return S_OK;
}

CModel_Instance* CModel_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const SAVE_MODEL& pModelData, const CVIBuffer_Instance::INSTANCE_DESC* pDesc)
{
	CModel_Instance* pInstance = new CModel_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelData, pDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel_Instance"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CModel_Instance::Clone(void* pArg)
{
	CModel_Instance* pInstance = new CModel_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_MesCModel_Instance_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pInstance_Buffer);

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);
	m_Materials.clear();


}
