#include "pch.h"
#include "Edit_Bone.h"

CEdit_Bone::CEdit_Bone()
{
}


HRESULT CEdit_Bone::Initialize(const aiNode* pAINode, _int iParentBoneIndex, SAVE_MODEL* pModelData)
{
	//�� �̸��� ����
	strcpy_s(m_szName, pAINode->mName.data);

	int iLength = MultiByteToWideChar(CP_ACP, 0, m_szName, -1, nullptr, 0);
	wstring szBoneName(iLength, L'\0');
	MultiByteToWideChar(CP_ACP, 0, m_szName, -1, &szBoneName[0], iLength);

	if (!szBoneName.empty() && szBoneName.back() == L'\0')
		szBoneName.pop_back();

	m_Bone.szName = szBoneName;

	//�� ����� �����´�.
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	//aissmp ����� �� �������� �Ǿ� �־ ��ġ ����
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	m_Bone.TransformationMatrix = m_TransformationMatrix;
	//�׵� ��ķ� �ʱ�ȭ
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	//�θ� �� �ε����� �޾ƿ´�.
	m_Bone.iParentBoneIndex = m_iParentBoneIndex = iParentBoneIndex;

	pModelData->Bones.push_back(m_Bone);
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

CEdit_Bone* CEdit_Bone::Create(const aiNode* pAINode, _int iParentBoneIndex, SAVE_MODEL* pModelData)
{
	CEdit_Bone* pInstance = new CEdit_Bone();

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex, pModelData)))
	{
		MSG_BOX(TEXT("Failed to Created : CEdit_Bone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEdit_Bone* CEdit_Bone::Clone()
{
	return new CEdit_Bone(*this);
}

void CEdit_Bone::Free()
{
	__super::Free();
}
