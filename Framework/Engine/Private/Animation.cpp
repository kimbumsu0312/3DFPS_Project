#include "pch.h"
#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& Prototype) : m_fDuration(Prototype.m_fDuration)
    , m_fTickPerSecond(Prototype.m_fTickPerSecond), m_fCurrentTrackPosition(Prototype.m_fCurrentTrackPosition)
    , m_iNumChannels(Prototype.m_iNumChannels), m_Channels(Prototype.m_Channels)
    , m_CurrentKeyFrameIndices(Prototype.m_CurrentKeyFrameIndices), m_KeyFrames(Prototype.m_KeyFrames), m_PreKeyFrameIndices(Prototype.m_PreKeyFrameIndices)
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
    m_PreKeyFrameIndices.resize(m_iNumChannels);
    m_KeyFrames.resize(m_iNumChannels);

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        CChannel* pChannel = CChannel::Create(pAnimation.Channels[i], Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

    return S_OK;
}

void CAnimation::Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, _bool isLoop, ANIM_STATUS eAnimStatus, _bool* pFinished, _bool IsAnimChange, const ANIMEFRAME& pAnimFrameData)
{
    if (ANIM_STATUS::PLAY == eAnimStatus)
        m_fCurrentTrackPosition += pAnimFrameData.fTickPerSecond * fTimeDelta;
    if (IsAnimChange && !m_bIsAnimChange)
    {
        Anim_Change_Reset();
        m_fCurrentTrackPosition = pAnimFrameData.iStartFrame;
        m_bIsAnimChange = true;
    }

    if (m_fTransitionTime < m_fTransitionDuration)
    {

        m_fTransitionTime += fTimeDelta;
        _float fRatio = min(m_fTransitionTime / m_fTransitionDuration, 1.f);

        for (_int i = 0; i < m_iNumChannels; ++i)
        {
            m_Channels[i]->Update_TransformationMatirx_AnimChange(Bones, m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[i], m_KeyFrames[i], fRatio);
        }

        if (m_fTransitionTime >= m_fTransitionDuration)
            m_bIsAnimChange = false;

    }
    else if (m_fCurrentTrackPosition >= pAnimFrameData.iEndFrame)
    {
        *pFinished = true;
        if (false == isLoop)
        {
            m_fCurrentTrackPosition = pAnimFrameData.iEndFrame;
            m_fPreTrackPosition = m_fCurrentTrackPosition;
            return;
        }
        else
            m_fCurrentTrackPosition = pAnimFrameData.iStartFrame;
    
    }
    else if (m_fCurrentTrackPosition < pAnimFrameData.iStartFrame)
    {
        m_fCurrentTrackPosition = pAnimFrameData.iStartFrame;
    }
    
    for (_uint i = 0; i < m_iNumChannels; ++i)
    {
        m_Channels[i]->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition, m_fPreTrackPosition, &m_CurrentKeyFrameIndices[i], pAnimFrameData);
    }
    m_fPreTrackPosition = m_fCurrentTrackPosition;
    
}

void CAnimation::Anim_Change_Set(const vector<class CBone*>& Bones, const ANIMEFRAME& pAnimFrameData)
{
    m_fCurrentTrackPosition = pAnimFrameData.iStartFrame;
    for (_int i = 0; i < m_iNumChannels; ++i)
    {
        m_CurrentKeyFrameIndices[i] = 0;
        m_Channels[i]->Anim_Change_Set(Bones, m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[i]);
    }
}
void CAnimation::Anim_Change_Reset()
{
    m_fTransitionTime = 0.f;
    m_bIsAnimChange = true;
    for (_int i = 0; i < m_KeyFrames.size(); ++i)
    {
        m_KeyFrames[i] = m_Channels[i]->Get_PreKeyFream(m_PreKeyFrameIndices[i]);
        m_CurrentKeyFrameIndices[i] = 0;
    }
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
