#include "pch.h"
#include "Edit_Model.h"
#include "Edit_Mesh.h"
#include "Edit_MeshMaterial.h"
#include "Edit_Bone.h"
#include "Edit_Animation.h"
#include "Transform.h"

CEdit_Model::CEdit_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext}
{
}

CEdit_Model::CEdit_Model(const CEdit_Model& Prototype) : CComponent(Prototype), m_Meshes(Prototype.m_Meshes)
, m_Materials(Prototype.m_Materials), m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
, m_ModelData(Prototype.m_ModelData), m_iCurrentAnimIndex(Prototype.m_iCurrentAnimIndex), m_iNumAnimations(Prototype.m_iNumAnimations)
{
	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.push_back(pPrototypeBone->Clone());
	
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

	for (auto& pPrototypeAnimation : Prototype.m_Animations)
		m_Animations.push_back(pPrototypeAnimation->Clone());

}

HRESULT CEdit_Model::Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, void* pArg)
{
	MODEl_DESC* pDesc = static_cast<MODEl_DESC*>(pArg);

	m_ModelData.szName = pDesc->szModelName;
	m_ModelData.eModel = eModelType;
	
	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
	m_ModelData.PreTransformMatrix = m_PreTransformMatrix;
	//파일을 어떻게 불러올지 플래그 셋팅
	_uint iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };
	
	if (MODELTYPE::NONANIM == m_ModelData.eModel)
		iFlag |= aiProcess_PreTransformVertices;

	//파일 로드
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEdit_Model::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CEdit_Model::Bind_Materials(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eTextureType, _uint iIndex)
{
	if (iMeshIndex >= m_ModelData.iNumMeshes)
		return E_FAIL;

	_uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (m_ModelData.iNumMaterials <= iMaterialIndex)
		return E_FAIL;

	return m_Materials[iMaterialIndex]->Bind_Shader_Resource(pShader, pConstantName, eTextureType, iIndex);
}

HRESULT CEdit_Model::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	if (iMeshIndex >= m_ModelData.iNumMeshes)
		return E_FAIL;

	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader, pConstantName, m_Bones);
}

_bool CEdit_Model::Play_Animation(_float fTimeDelta, _bool bIsAnimStop)
{
	if (m_ModelData.eModel != MODELTYPE::ANIM)
		return false;
	
	m_bisFinished = false;

	//현재 애니메이션에 뼈 트랜슾폼 매트릭스를 갱신
	m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, fTimeDelta,m_bisLoop, &m_bisFinished, bIsAnimStop);

	for (auto& pBone : m_Bones)
	{
		//뼈들의 행렬을 부모 뼈의 행렬에 맞게 맞춰준다.
		pBone->Update_CombinedTransformationMatrix(m_PreTransformMatrix, m_Bones);
	}

	return m_bisFinished;
}

_bool CEdit_Model::Play_Animation(_float fTimeDelta, _bool bIsAnimStop, _int iStartFrame, _int iEndFrame)
{
	if (m_ModelData.eModel != MODELTYPE::ANIM)
		return false;

	m_bisFinished = false;
	m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, fTimeDelta, m_bisLoop, &m_bisFinished, bIsAnimStop, iStartFrame, iEndFrame);

	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_PreTransformMatrix, m_Bones);
	}

	return m_bisFinished;
}

_bool CEdit_Model::Selete_Model(CTransform& pTransform, _float3& pOut)
{
	for (_int i = 0; i < m_Meshes.size(); ++i)
	{
		if (m_Meshes[i]->IsPicked(m_ModelData.eModel, pTransform, pOut))
			return true;
	}

	return false;
}

void CEdit_Model::Set_Animations(_uint iIndex, _bool IsLoop)
{
	if (m_ModelData.eModel != MODELTYPE::ANIM || iIndex >= m_iNumAnimations)
		return;
	
	m_iCurrentAnimIndex = iIndex; 
	m_bisLoop = IsLoop;
}

void CEdit_Model::Set_Animation(_int i, _float fTickPerSecond_float, _float fCurrentTrackPosition)
{
	if (i == 0)
		m_Animations[0]->Set_TickPerSecond(fTickPerSecond_float);
	else if (i == 1)
		m_Animations[0]->Set_CurTrackPositon(fCurrentTrackPosition);
	else
		m_Animations[0]->Add_CurTrackPositon((_int)fCurrentTrackPosition);
}

_float CEdit_Model::Get_Animation(_int i)
{
	if (i == 0)
		return m_Animations[0]->Get_TickPerSecond();
	else if (i == 1)
		return m_Animations[0]->Get_CurTrackPositon();
	else
		return m_Animations[0]->Get_Duration();
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
	//매쉬 이름 복사
	m_ModelData.iNumMeshes = m_pAIScene->mNumMeshes;
	
	for (_uint i = 0; i < m_ModelData.iNumMeshes; ++i)
	{
		CEdit_Mesh* pMesh = CEdit_Mesh::Create(m_pDevice, m_pContext, m_ModelData.eModel, m_pAIScene->mMeshes[i], m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix), &m_ModelData);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

HRESULT CEdit_Model::Ready_Materials(const _char* pModelFilePath)
{
	m_ModelData.iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_ModelData.iNumMaterials; i++)
	{

		CEdit_MeshMaterial* pMeshMaterial = CEdit_MeshMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i], &m_ModelData);
		if (nullptr == pMeshMaterial)
			return E_FAIL;

		m_Materials.push_back(pMeshMaterial);
	}


	return S_OK;
}

HRESULT CEdit_Model::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
	
	CEdit_Bone* pBone = CEdit_Bone::Create(pAINode, iParentIndex, &m_ModelData );
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	//부모 뼈 인덱스
	_int   iIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(pAINode->mChildren[i], iIndex);
	}

	m_ModelData.iNumBone = m_Bones.size();
	return S_OK;
}

HRESULT CEdit_Model::Ready_Animations()
{
	//파일의 애니메이션 개수를 가져온다.
	m_iNumAnimations = m_pAIScene->mNumAnimations;
	
	for (_int i = 0; i < m_iNumAnimations; ++i)
	{
		//i번째 애니메이션을 생성한다.
		CEdit_Animation* pAnimation = CEdit_Animation::Create(m_pAIScene->mAnimations[i], m_Bones, &m_ModelData);
		if (pAnimation == nullptr)
			return E_FAIL;

		//생성한 애니메이션을 넣어줌
		m_Animations.push_back(pAnimation);
	}
	m_ModelData.iNumAnimations = m_Animations.size();
	return S_OK;
}

CEdit_Model* CEdit_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, void* pArg)
{
	CEdit_Model* pInstance = new CEdit_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix, pArg)))
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

	for (auto& pAnim : m_Animations)
		Safe_Release(pAnim);
	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);
	m_Materials.clear();

	m_Importer.FreeScene();

	
}
