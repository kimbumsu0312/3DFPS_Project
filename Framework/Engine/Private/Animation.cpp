#include "pch.h"
#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& Prototype) : m_fDuration(Prototype.m_fDuration)
    , m_fTickPerSecond(Prototype.m_fTickPerSecond), m_fCurrentTrackPosition(Prototype.m_fCurrentTrackPosition)
    , m_iNumChannels(Prototype.m_iNumChannels), m_Channels(Prototype.m_Channels)
    , m_CurrentKeyFrameIndices(Prototype.m_CurrentKeyFrameIndices)
{
    for (auto& pChannel : m_Channels)
        Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones)
{
    m_iNumChannels = pAnimation.iNumChannels;
    m_fDuration = pAnimation.fDuration;
    m_fTickPerSecond = pAnimation.fTickPerSecond;

    m_CurrentKeyFrameIndices.resize(m_iNumChannels);

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        CChannel* pChannel = CChannel::Create(pAnimation.Channels[i], Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

    return S_OK;
}

void CAnimation::Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, _bool isLoop, ANIM_STATUS eAnimStatus, _bool* pFinished, const ANIMEFRAME& pAnimFrameData)
{
    if(ANIM_STATUS::PLAY == eAnimStatus )
        m_fCurrentTrackPosition += pAnimFrameData.fTickPerSecond * fTimeDelta;

    if (m_fCurrentTrackPosition >= pAnimFrameData.iEndFrame)
    {
        if (false == isLoop)
        {
            *pFinished = true;
            m_fCurrentTrackPosition = pAnimFrameData.iEndFrame;
            m_fPreTrackPosition = m_fCurrentTrackPosition;
            return;
        }
        else
            m_fCurrentTrackPosition = pAnimFrameData.iStartFrame;

    }
    for (_uint i = 0; i < m_iNumChannels; ++i)
    {
        m_Channels[i]->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition, m_fPreTrackPosition, &m_CurrentKeyFrameIndices[i]);
    }
    m_fPreTrackPosition = m_fCurrentTrackPosition;
}


CAnimation* CAnimation::Create(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(pAnimation, Bones)))
    {
        MSG_BOX(TEXT("Failed to Created : CAnimation"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CAnimation* CAnimation::Clone()
{
    return new CAnimation(*this);
}

void CAnimation::Free()
{
    __super::Free();

    for (auto& pChannel : m_Channels)
        Safe_Release(pChannel);

    m_Channels.clear();
}
