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

void CChannel::Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _float fPreTrackPosition, _uint* pCurrentKeyFrameIndex, const ANIMEFRAME& pAnimFrameData)
{
    const _uint iStartIndex = Finde_KeyFrameIndex(pAnimFrameData.iStartFrame);
    const _uint iEndIndex = Finde_KeyFrameIndex(pAnimFrameData.iEndFrame);

    _vector         vScale, vRotation, vTranslation;
    KEYFRAME        StartKeyFrame = m_KeyFrames[iStartIndex];
    KEYFRAME        LastKeyFrame = m_KeyFrames[iEndIndex];

    if (fCurrentTrackPosition == pAnimFrameData.iStartFrame)
        *pCurrentKeyFrameIndex = iStartIndex;
    
    if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
    {
        vScale = XMLoadFloat3(&LastKeyFrame.vScale);
        vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
    }
    else if(*pCurrentKeyFrameIndex == iEndIndex)
    {
        vScale = XMLoadFloat3(&LastKeyFrame.vScale);
        vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
    }
    else if (fPreTrackPosition > fCurrentTrackPosition)
    {
        vScale = XMLoadFloat3(&StartKeyFrame.vScale);
        vRotation = XMLoadFloat4(&StartKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&StartKeyFrame.vTranslation), 1.f);
    }
    else
    {
        while (fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition)
            ++*pCurrentKeyFrameIndex;

        _float fRatio = (fCurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition) / (m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition);

        vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale), fRatio);
        vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation), fRatio);
        vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);
    
    }

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Update_TransformationMatirx_AnimChange(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const KEYFRAME& preKeyFrame, _float fRatio)
{
    _vector vScale, vRotation, vTranslation;

    const _uint iEndIndex = Finde_KeyFrameIndex(fCurrentTrackPosition);

    vScale = XMVectorLerp(XMLoadFloat3(&preKeyFrame.vScale), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale), fRatio);
    vRotation = XMQuaternionSlerp(XMLoadFloat4(&preKeyFrame.vRotation), XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation), fRatio);
    vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&preKeyFrame.vTranslation), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation), fRatio), 1.f);

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Anim_Change_Set(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex)
{
    _vector vScale, vRotation, vTranslation;

    while (fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition)
        ++*pCurrentKeyFrameIndex;

    vScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale);
    vRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation);
    vTranslation = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation);

    Bones[m_iBoneIndex]->Set_NewTransitionTransformationMatrix(XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vRotation, vTranslation));
}

_uint CChannel::Finde_KeyFrameIndex(_float fTrackPotion)
{
    for (_uint i = 0; i + 1 < m_KeyFrames.size(); ++i)
    {
        if (fTrackPotion < m_KeyFrames[i + 1].fTrackPosition)
            return i;
    }
    return static_cast<_uint>(m_KeyFrames.size() - 1);
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

