#include "pch.h"
#include "Edit_Bone.h"

CEdit_Bone::CEdit_Bone()
{
}

HRESULT CEdit_Bone::Initialize(const aiNode* pAINode)
{
	strcpy_s(m_szName, pAINode->mName.data);
	
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

CEdit_Bone* CEdit_Bone::Create(const aiNode* pAINode)
{
	CEdit_Bone* pInstance = new CEdit_Bone();

	if (FAILED(pInstance->Initialize(pAINode)))
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
