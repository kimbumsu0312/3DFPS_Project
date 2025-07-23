#include "pch.h"
#include "Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const SAVE_CHANNEL& pChannel, const vector<class CBone*>& Bones)
{
    m_iBoneIndex = pChannel.iBoneIndex;
    m_iNumKeyFrames = pChannel.iNumKeyFrames;
 
    _float3     vScale{};
    _float4     vRotation{};
    _float3     vTranslation{};

    for (size_t i = 0; i < m_iNumKeyFrames; i++)
    {
        KEYFRAME            KeyFrame{};
       
        KeyFrame.vScale = pChannel.KeyFrames[i].vScale;
        KeyFrame.vRotation = pChannel.KeyFrames[i].vRotation;
        KeyFrame.vTranslation = pChannel.KeyFrames[i].vTranslation;
        KeyFrame.fTrackPosition = pChannel.KeyFrames[i].fTrackPosition;

        m_KeyFrames.push_back(KeyFrame);
    }
    return S_OK;
}

void CChannel::Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition)
{
    _float fCurrentTrackPos = fCurrentTrackPosition;

    //현재 뼈들의 행렬을 구해온다.
    _vector vScale, vRotation, vTranslation;

    _int iCulIndex = (_int)fCurrentTrackPos;
    _int iNextIndex = iCulIndex + 1;
    _float fFrame = fCurrentTrackPos - iCulIndex;

    if (iNextIndex < m_KeyFrames.size() - 1)
    {
        vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCulIndex].vScale), XMLoadFloat3(&m_KeyFrames[iNextIndex].vScale), fFrame);
        vTranslation = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCulIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[iNextIndex].vTranslation), fFrame);
        vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[iCulIndex].vRotation), XMLoadFloat4(&m_KeyFrames[iNextIndex].vRotation), fFrame);
    }
    else
    {
        vScale = XMLoadFloat3(&m_KeyFrames[m_KeyFrames.size() - 1].vScale);
        vTranslation = XMLoadFloat3(&m_KeyFrames[m_KeyFrames.size() - 1].vTranslation);
        vRotation = XMLoadFloat4(&m_KeyFrames[m_KeyFrames.size() - 1].vRotation);
    }

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

CChannel* CChannel::Create(const SAVE_CHANNEL& pChannel, const vector<class CBone*>& Bones)
{
    CChannel* pInstance = new CChannel();

    if (FAILED(pInstance->Initialize(pChannel, Bones)))
    {
        MSG_BOX(TEXT("Failed to Created : CChannel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChannel::Free()
{
    __super::Free();

    m_KeyFrames.clear();
}

