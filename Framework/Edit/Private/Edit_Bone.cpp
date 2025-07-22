#include "pch.h"
#include "Edit_Bone.h"

CEdit_Bone::CEdit_Bone()
{
}


HRESULT CEdit_Bone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	//뼈 이름을 복사
	strcpy_s(m_szName, pAINode->mName.data);

	//뼈 행렬을 가져온다.
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	//aissmp 행렬이 콜 매이저로 되어 있어서 전치 해줌
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	//항등 행렬로 초기화
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	//부모 뼈 인덱스를 받아온다.
	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CEdit_Bone::Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CEdit_Bone*>& Bones)
{
	//부모 뼈 인덱스가 -1이면 부모뼈가 없는 걸로 판단
	if (-1 == m_iParentBoneIndex)
	{
		//m_CombinedTransformationMatrix에 임의로 맞춰둔 행렬 * 자신의 트랜스폼 행렬
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&PreTransformMatrix) * XMLoadFloat4x4(&m_TransformationMatrix));
		return;
	}

	//부모가 있는 경우
	// 자신의 m_CombinedTransformationMatrix에 자신 트랜스폼 행렬 * 부모의 콤바인드 행렬을 넣어줌
	XMStoreFloat4x4(&m_CombinedTransformationMatrix,
		XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
}

CEdit_Bone* CEdit_Bone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
	CEdit_Bone* pInstance = new CEdit_Bone();

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
	{
		MSG_BOX(TEXT("Failed to Created : CEdit_Bone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEdit_Bone::Free()
{
	__super::Free();
}
