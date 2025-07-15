#include "pch.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshMaterial.h"
CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext}
{
}

CModel::CModel(const CModel& Prototype) : CComponent(Prototype), m_Meshes(Prototype.m_Meshes), m_iNumMeshes(Prototype.m_iNumMeshes), m_eModelType ( Prototype.m_eModelType),
m_iNumMaterials(Prototype.m_iNumMaterials), m_Materials(Prototype.m_Materials)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);
}

HRESULT CModel::Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	m_eModelType = eModelType;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	//������ ��� �ҷ����� �÷��� ����
	_uint iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };
	
	if (MODELTYPE::NONANIM == m_eModelType)
		iFlag |= aiProcess_PreTransformVertices;

	//���� �ε�
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, const _char* pConstantName, _uint iSRVIndex, _uint iShaderIndex)
{
	for (auto& pMesh : m_Meshes)
	{
		m_Materials[pMesh->Get_MaterialIndex()]->Bind_Shader_Resource(pShader, pConstantName, iSRVIndex);

		pShader->Begin(iShaderIndex);
		if (FAILED(pMesh->Bind_Resources()))
			return E_FAIL;

		if (FAILED(pMesh->Render()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;
	
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMeshes = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshMaterial* pMeshMaterial = CMeshMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i]);
		if (nullptr == pMeshMaterial)
			return E_FAIL;

		m_Materials.push_back(pMeshMaterial);

	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
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
