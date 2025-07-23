#include "pch.h"
#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const SAVE_BONE& pBone)
{
    m_szName = pBone.szName.c_str();
    m_TransformationMatrix = pBone.TransformationMatrix;
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
    m_iParentBoneIndex = pBone.iParentBoneIndex;

    return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones)
{
    if (-1 == m_iParentBoneIndex)
    {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&PreTransformMatrix) * XMLoadFloat4x4(&m_TransformationMatrix));
        return;
    }

    XMStoreFloat4x4(&m_CombinedTransformationMatrix,
        XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
}

CBone* CBone::Create(const SAVE_BONE& pBone)
{
    CBone* pInstance = new CBone();

    if (FAILED(pInstance->Initialize(pBone)))
    {
        MSG_BOX(TEXT("Failed to Created : CBone"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBone::Free()
{
    __super::Free();
}
