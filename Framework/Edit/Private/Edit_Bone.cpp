#include "pch.h"
#include "Edit_Bone.h"

CEdit_Bone::CEdit_Bone()
{
}


HRESULT CEdit_Bone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	//�� �̸��� ����
	strcpy_s(m_szName, pAINode->mName.data);

	//�� ����� �����´�.
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	//aissmp ����� �� �������� �Ǿ� �־ ��ġ ����
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	//�׵� ��ķ� �ʱ�ȭ
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	//�θ� �� �ε����� �޾ƿ´�.
	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CEdit_Bone::Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CEdit_Bone*>& Bones)
{
	//�θ� �� �ε����� -1�̸� �θ���� ���� �ɷ� �Ǵ�
	if (-1 == m_iParentBoneIndex)
	{
		//m_CombinedTransformationMatrix�� ���Ƿ� ����� ��� * �ڽ��� Ʈ������ ���
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&PreTransformMatrix) * XMLoadFloat4x4(&m_TransformationMatrix));
		return;
	}

	//�θ� �ִ� ���
	// �ڽ��� m_CombinedTransformationMatrix�� �ڽ� Ʈ������ ��� * �θ��� �޹��ε� ����� �־���
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
