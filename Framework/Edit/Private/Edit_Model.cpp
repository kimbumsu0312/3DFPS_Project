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

HRESULT CEdit_Model::Initialize_Prototype(const SAVE_MODEL& pModelData)
{
	m_ModelData.szName = pModelData.szName;
	m_ModelData.eModel = pModelData.eModel;
	m_PreTransformMatrix = pModelData.PreTransformMatrix;

	if (FAILED(Ready_Bones(pModelData)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(pModelData)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelData)))
		return E_FAIL;

	if (FAILED(Ready_Animations(pModelData)))
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

_bool CEdit_Model::Selete_Model(CTransform& pTransform, _float& pOut)
{
	_float fSourcelength = { -1.f };
	_bool IsPicked = false;

	for (_int i = 0; i < m_Meshes.size(); ++i)
	{
		_float fDestlength = { -1.f };

		if (true == m_Meshes[i]->IsPicked(m_ModelData.eModel, pTransform, fDestlength))
		{
			if (fSourcelength <= 0.f || fSourcelength > fDestlength)
			{
				fSourcelength = fDestlength;
				pOut = fSourcelength;
			}
			IsPicked = true;
		}

	}

	//if (IsPicked)
	//{
	//	XMVECTOR vWorldPos = XMVector3TransformCoord(XMLoadFloat3(&pOut), pTransform.Get_WorldMatrix());
	//	XMStoreFloat3(&pOut, vWorldPos);
	//}
	return IsPicked;
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
		m_Animations[m_iCurrentAnimIndex]->Set_TickPerSecond(fTickPerSecond_float);
	else if (i == 1)
		m_Animations[m_iCurrentAnimIndex]->Set_CurTrackPositon(fCurrentTrackPosition);
	else
		m_Animations[m_iCurrentAnimIndex]->Add_CurTrackPositon((_int)fCurrentTrackPosition);
}

_float CEdit_Model::Get_Animation(_int i)
{
	if (i == 0)
		return m_Animations[m_iCurrentAnimIndex]->Get_TickPerSecond();
	else if (i == 1)
		return m_Animations[m_iCurrentAnimIndex]->Get_CurTrackPositon();
	else
		return m_Animations[m_iCurrentAnimIndex]->Get_Duration();
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

HRESULT CEdit_Model::Ready_Meshes(const SAVE_MODEL& pModelData)
{
	m_ModelData.iNumMeshes = pModelData.iNumMeshes;

	for (size_t i = 0; i < m_ModelData.iNumMeshes; i++)
	{
		CEdit_Mesh* pMesh = CEdit_Mesh::Create(m_pDevice, m_pContext, m_ModelData.eModel, pModelData.Meshs[i], m_Bones, pModelData.Bones);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CEdit_Model::Ready_Materials(const SAVE_MODEL& pModelData)
{
	m_ModelData.iNumMaterials = pModelData.iNumMaterials;

	for (size_t i = 0; i < m_ModelData.iNumMaterials; i++)
	{

		CEdit_MeshMaterial* pMeshMaterial = CEdit_MeshMaterial::Create(m_pDevice, m_pContext, pModelData.MeshMaterials[i]);
		if (nullptr == pMeshMaterial)
			return E_FAIL;

		m_Materials.push_back(pMeshMaterial);
	}

	return S_OK;
}

HRESULT CEdit_Model::Ready_Bones(const SAVE_MODEL& pModelData)
{
	for (auto& BoneData : pModelData.Bones)
	{
		CEdit_Bone* pBone = CEdit_Bone::Create(BoneData);

		if (pBone == nullptr)
			return E_FAIL;

		m_Bones.push_back(pBone);
	}

	return S_OK;
}

HRESULT CEdit_Model::Ready_Animations(const SAVE_MODEL& pModelData)
{
	m_iNumAnimations = pModelData.iNumAnimations;;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CEdit_Animation* pAnimation = CEdit_Animation::Create(pModelData.Animations[i], m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

SAVE_MODEL CEdit_Model::Load_File(string szFilePath)
{
	string DataFilePath = {};
	SAVE_MODEL ModelData = {};

	ifstream file(szFilePath);
	if (!file.is_open())
	{
		MSG_BOX(TEXT("불러오기 실패"));
		return ModelData;
	}

	json jData;
	file >> jData;
	file.close();

	ModelData.szName = jData["Model_name"];
	ModelData.eModel = jData["Model_type"];
	ModelData.iNumMaterials = jData["iNumMaterials"];
	DataFilePath = jData["Data_Path"];


	size_t szSlash = szFilePath.find_last_of("/\\");
	string directory = (szSlash == string::npos) ? "" : szFilePath.substr(0, szSlash);

	szSlash = DataFilePath.find_last_of("/\\");
	string filename = (szSlash == string::npos) ? DataFilePath : DataFilePath.substr(szSlash + 1);

	if (directory.back() == '/' || directory.back() == '\\')
		DataFilePath = directory + filename;
	else
		DataFilePath = directory + "/" + filename;

	for (int i = 0; i < ModelData.iNumMaterials; ++i)
	{
		SAVE_MESHMATERIAL MeshMaterial;
		const auto& jMeshMaterial = jData["MeshMaterial"][i];

		for (const auto& jMaterial : jMeshMaterial["Materials"])
		{
			SAVE_MATERIAL material;
			material.iTexCount = jMaterial["iTexCount"];

			for (int k = 0; k < material.iTexCount; ++k)
			{
				string path = jMaterial["szFullPath"][k]["Path"];
				material.szFullPath.push_back(path);
			}

			MeshMaterial.Materials.push_back(material);
		}

		ModelData.MeshMaterials.push_back(MeshMaterial);
	}


	ifstream DatFile(DataFilePath, ios::binary);
	if (!DatFile)
	{
		MSG_BOX(TEXT("데이터 파일 불러오기 실패"));
		return ModelData;
	}

	DatFile.read(reinterpret_cast<_char*>(&ModelData.PreTransformMatrix), sizeof(_float4x4));

	size_t iMeshCount = 0;
	DatFile.read(reinterpret_cast<_char*>(&iMeshCount), sizeof(size_t));
	ModelData.iNumMeshes = static_cast<_int>(iMeshCount);
	ModelData.Meshs.resize(iMeshCount);
	//매쉬 로드
	for (auto& mesh : ModelData.Meshs)
	{
		size_t nameSize = 0;
		DatFile.read(reinterpret_cast<_char*>(&nameSize), sizeof(size_t));

		_wstring name(nameSize, L'\0');
		DatFile.read(reinterpret_cast<_char*>(&name[0]), nameSize * sizeof(_tchar));
		mesh.szName = name;

		DatFile.read(reinterpret_cast<_char*>(&mesh.iMaterialIndex), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iNumVertices), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iVertexStride), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iNumIndices), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iNumFaces), sizeof(_uint));

		size_t faceCount = 0;
		DatFile.read(reinterpret_cast<_char*>(&faceCount), sizeof(size_t));
		mesh.iFaces.resize(faceCount);
		DatFile.read(reinterpret_cast<_char*>(mesh.iFaces.data()), faceCount * sizeof(Face));

		if (MODELTYPE::ANIM == ModelData.eModel)
		{
			size_t vertexCount = 0;
			DatFile.read(reinterpret_cast<_char*>(&vertexCount), sizeof(size_t));
			mesh.AnimVertex.resize(vertexCount);
			DatFile.read(reinterpret_cast<_char*>(mesh.AnimVertex.data()), vertexCount * sizeof(VTXANIMMESH));

			DatFile.read(reinterpret_cast<_char*>(&mesh.iNumBones), sizeof(_uint));

			size_t boneIndexCount = 0;
			DatFile.read(reinterpret_cast<_char*>(&boneIndexCount), sizeof(size_t));
			mesh.BoneIndices.resize(boneIndexCount);
			DatFile.read(reinterpret_cast<_char*>(mesh.BoneIndices.data()), boneIndexCount * sizeof(_int));
		}
		else
		{
			size_t vertexCount = 0;
			DatFile.read(reinterpret_cast<_char*>(&vertexCount), sizeof(size_t));
			mesh.NonAnimVertex.resize(vertexCount);
			DatFile.read(reinterpret_cast<_char*>(mesh.NonAnimVertex.data()), vertexCount * sizeof(VTXMESH));
		}
	}

	if (ModelData.eModel != MODELTYPE::ANIM)
	{
		DatFile.close();
		return ModelData;
	}

	//뼈 로드
	size_t iBoneCount = 0;
	DatFile.read(reinterpret_cast<_char*>(&iBoneCount), sizeof(size_t));
	ModelData.iNumBone = static_cast<_int>(iBoneCount);
	ModelData.Bones.resize(iBoneCount);

	for (auto& Bone : ModelData.Bones)
	{
		size_t nameSize = 0;
		DatFile.read(reinterpret_cast<_char*>(&nameSize), sizeof(size_t));

		vector<_tchar> buffer(nameSize);
		DatFile.read(reinterpret_cast<_char*>(buffer.data()), nameSize * sizeof(_tchar));
		Bone.szName.assign(buffer.data(), nameSize);

		DatFile.read(reinterpret_cast<_char*>(&Bone.TransformationMatrix), sizeof(_float4x4));
		DatFile.read(reinterpret_cast<_char*>(&Bone.matOffset), sizeof(_float4x4));
		DatFile.read(reinterpret_cast<_char*>(&Bone.iParentBoneIndex), sizeof(_int));
	}

	//애니메이션 로드
	size_t iAnimCount = 0;
	DatFile.read(reinterpret_cast<_char*>(&iAnimCount), sizeof(size_t));
	ModelData.iNumAnimations = static_cast<_int>(iAnimCount);
	ModelData.Animations.resize(iAnimCount);

	for (auto& Anim : ModelData.Animations)
	{
		DatFile.read(reinterpret_cast<_char*>(&Anim.iNumChannels), sizeof(_int));
		DatFile.read(reinterpret_cast<_char*>(&Anim.fDuration), sizeof(_float));
		DatFile.read(reinterpret_cast<_char*>(&Anim.fTickPerSecond), sizeof(_float));

		Anim.Channels.resize(Anim.iNumChannels);
		for (auto& Channel : Anim.Channels)
		{
			DatFile.read(reinterpret_cast<_char*>(&Channel.iBoneIndex), sizeof(_int));
			DatFile.read(reinterpret_cast<_char*>(&Channel.iNumKeyFrames), sizeof(_int));

			Channel.KeyFrames.resize(Channel.iNumKeyFrames);
			for (auto& Keyframe : Channel.KeyFrames)
			{
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.vScale), sizeof(_float3));
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.vRotation), sizeof(_float4));
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.vTranslation), sizeof(_float3));
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.fTrackPosition), sizeof(_float));
			}
		}
	}

	DatFile.close();

	return ModelData;
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

CEdit_Model* CEdit_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string szFilePath)
{
	CEdit_Model* pInstance = new CEdit_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pInstance->Load_File(szFilePath))))
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
