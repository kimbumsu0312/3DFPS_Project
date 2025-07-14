#include "pch.h"
#include "Model.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext}
{
}

CModel::CModel(const CModel& Prototype) : CComponent(Prototype), m_Meshes(Prototype.m_Meshes)
{
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{
	//파일을 어떻게 불러올지 플래그 셋팅
	_uint iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };
	
	//파일 로드
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_pAIScene->mAnimations;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint Index)
{

	if(FAILED(m_Meshes[Index]->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_Meshes[Index]->Render()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i]);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath)))
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

	for (auto& pLight : m_Meshes)
		Safe_Release(pLight);

	m_Meshes.clear();

	m_Importer.FreeScene();

	
}
