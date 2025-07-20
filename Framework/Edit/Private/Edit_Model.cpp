#include "pch.h"
#include "Engine_Defines.h"
#include "Edit_Model.h"
#include "Edit_Mesh.h"
#include "Edit_MeshMaterial.h"
CEdit_Model::CEdit_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext}
{
}

CEdit_Model::CEdit_Model(const CEdit_Model& Prototype) : CComponent(Prototype), m_Meshes(Prototype.m_Meshes), m_iNumMeshes(Prototype.m_iNumMeshes), m_eModelType ( Prototype.m_eModelType),
m_iNumMaterials(Prototype.m_iNumMaterials), m_Materials(Prototype.m_Materials), m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
, m_ModelData(Prototype.m_ModelData)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);
}

HRESULT CEdit_Model::Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	m_eModelType = m_ModelData.eModel = eModelType;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	//파일을 어떻게 불러올지 플래그 셋팅
	_uint iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };
	
	if (MODELTYPE::NONANIM == m_eModelType)
		iFlag |= aiProcess_PreTransformVertices;

	//파일 로드
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEdit_Model::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CEdit_Model::Bind_Materials(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eTextureType, _uint iIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (m_iNumMaterials <= iMaterialIndex)
		return E_FAIL;

	return m_Materials[iMaterialIndex]->Bind_Shader_Resource(pShader, pConstantName, eTextureType, iIndex);
}

_bool CEdit_Model::Selete_Model(CTransform& pTransform, _float3& pOut)
{
	for (_int i = 0; i < m_Meshes.size(); ++i)
	{
		if (m_Meshes[i]->IsPicked(pTransform, pOut))
			return true;
	}

	return false;
}

HRESULT CEdit_Model::Render(_uint iMeshIndex)
{
	if (FAILED(m_Meshes[iMeshIndex]->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_Meshes[iMeshIndex]->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEdit_Model::Ready_Meshes()
{
	m_iNumMeshes = m_ModelData.iNumMeshes = m_pAIScene->mNumMeshes;
	
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CEdit_Mesh* pMesh = CEdit_Mesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix), &m_ModelData);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

HRESULT CEdit_Model::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_ModelData.iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		CEdit_MeshMaterial* pMeshMaterial = CEdit_MeshMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i], &m_ModelData);
		if (nullptr == pMeshMaterial)
			return E_FAIL;

		m_Materials.push_back(pMeshMaterial);

	}

	return S_OK;
}

CEdit_Model* CEdit_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CEdit_Model* pInstance = new CEdit_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CEdit_Model"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CEdit_Model::Clone(void* pArg)
{
	CEdit_Model* pInstance = new CEdit_Model(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CEdit_Model"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEdit_Model::Free()
{
	__super::Free();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);

	m_Materials.clear();

	m_Importer.FreeScene();

	
}
