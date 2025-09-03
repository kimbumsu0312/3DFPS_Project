#include "pch.h"
#include "Bone.h"
#include "Transform.h"
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

void CBone::Set_CombindMationMatinMatrix_PosReset()
{
    _matrix Combined = XMLoadFloat4x4(&m_CombinedTransformationMatrix);
    Combined.r[3] = XMVectorSet(0.f, XMVectorGetY(Combined.r[3]), 0.f, 1.f);
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, Combined);
}

void CBone::Set_CombindMationMatinMatrix_RotReset(const _float4x4& PreTransformMatrix)
{
    _matrix Combined = XMLoadFloat4x4(&m_CombinedTransformationMatrix);
    _vector vScale, vRot, vTrans;
    XMMatrixDecompose(&vScale, &vRot, &vTrans, Combined);

    _vector vQuatScale, vQuatRot, vQuatTrans;
    XMMatrixDecompose(&vQuatScale, &vQuatRot, &vQuatTrans, XMLoadFloat4x4(&PreTransformMatrix));
    vRot = vQuatRot;
    Combined = XMMatrixAffineTransformation(vScale, XMVectorZero(), vRot, vTrans);

    XMStoreFloat4x4(&m_CombinedTransformationMatrix, Combined);
}

void CBone::Set_RotBonePitch(_float fPitch, _bool IsTrans)
{
    m_fPitch = fPitch;

}

void CBone::Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones)
{
    _matrix matPitch = XMMatrixRotationX(m_fPitch);
    if (-1 == m_iParentBoneIndex)
    {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&PreTransformMatrix) * matPitch * XMLoadFloat4x4(&m_TransformationMatrix));
        return;
    }

    XMStoreFloat4x4(&m_CombinedTransformationMatrix,
        XMLoadFloat4x4(&m_TransformationMatrix) * matPitch * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
}

void CBone::Update_CombinedTransformationMatrix_Transition(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones)
{
    _matrix matPitch = XMMatrixRotationX(m_fPitch);
    if (-1 == m_iParentBoneIndex)
    {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&PreTransformMatrix) * matPitch * XMLoadFloat4x4(&m_TransformationMatrix));
        return;
    }

    XMStoreFloat4x4(&m_CombinedTransformationMatrix,
        XMLoadFloat4x4(&m_CombinedTransformationMatrix) * matPitch * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
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

CBone* CBone::Clone()
{
    return new CBone(*this);
}

void CBone::Free()
{
    __super::Free();
}
