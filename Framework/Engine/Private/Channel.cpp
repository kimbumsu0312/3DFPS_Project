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

void CChannel::Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _float fPreTrackPosition, _uint* pCurrentKeyFrameIndex, const ANIMEFRAME& pAnimFrameData, _float fRatio)
{
    const _uint iStartIndex = Finde_KeyFrameIndex(pAnimFrameData.iStartFrame);
    const _uint iEndIndex = Finde_KeyFrameIndex(pAnimFrameData.iEndFrame);
    const _uint iCulIndex = Finde_KeyFrameIndex(fCurrentTrackPosition);

    _vector         vScale, vRotation, vTranslation;
    KEYFRAME        StartKeyFrame = m_KeyFrames[iStartIndex];
    KEYFRAME        LastKeyFrame = m_KeyFrames[iEndIndex];

    if (fCurrentTrackPosition == pAnimFrameData.iStartFrame)
        *pCurrentKeyFrameIndex = iStartIndex;

    if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition || *pCurrentKeyFrameIndex == iEndIndex)
    {
        vScale = XMLoadFloat3(&LastKeyFrame.vScale);
        vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
    }
    //else if (*pCurrentKeyFrameIndex == iEndIndex)
    //{
    //    vScale = XMVectorLerp(XMLoadFloat3(&StartKeyFrame.vScale), XMLoadFloat3(&LastKeyFrame.vScale), fRatio);
    //    vRotation = XMQuaternionSlerp(XMLoadFloat4(&StartKeyFrame.vRotation), XMLoadFloat4(&LastKeyFrame.vRotation), fRatio);
    //    vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&StartKeyFrame.vTranslation), XMLoadFloat3(&LastKeyFrame.vTranslation), fRatio), 1.f);
    //}
    else
    {
        if(fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition)
            *pCurrentKeyFrameIndex = Finde_KeyFrameIndex(fCurrentTrackPosition);

        _float fRatio = (fCurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition) / (m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition);

        vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale), fRatio);
        vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation), fRatio);
        vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);

    }

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Update_TransformationMatirx_Transition(const vector<class CBone*>& Bones, _uint pCurrentKeyFrameIndex, _float fRatio)
{

    const _uint iStartIndex = Finde_KeyFrameIndex(pCurrentKeyFrameIndex);
    _vector vScale, vRotation, vTranslation;

    if (!XMMatrixDecompose(&vScale, &vRotation, &vTranslation, Bones[m_iBoneIndex]->Get_TransformationMatrix()))
    {
        vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
        vRotation = XMQuaternionIdentity();
        vTranslation = XMVectorZero();
    }
    _vector vRotStart = vRotation;
    _vector vRotEnd = XMLoadFloat4(&m_KeyFrames[iStartIndex].vRotation);

    vScale = XMVectorLerp(vScale, XMLoadFloat3(&m_KeyFrames[iStartIndex].vScale), fRatio);
    vRotation = XMQuaternionSlerp(vRotation, XMLoadFloat4(&m_KeyFrames[iStartIndex].vRotation), fRatio);
    vTranslation = XMVectorSetW(XMVectorLerp(vTranslation, XMLoadFloat3(&m_KeyFrames[iStartIndex].vTranslation), fRatio), 1.f);

    //if (XMVectorGetX(XMVector4Dot(vRotStart, vRotEnd)) < 0.0f)
    //    vRotEnd = XMVectorNegate(vRotEnd);
    //
    //vRotation = XMQuaternionSlerp(vRotStart, vRotEnd, fRatio);
    //vRotation = XMQuaternionNormalize(vRotation);

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vRotation, vTranslation);
    
    Bones[m_iBoneIndex]->Set_CombinedTransformationMatrix(TransformationMatrix);
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

