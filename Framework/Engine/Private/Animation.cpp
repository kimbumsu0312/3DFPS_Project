#include "pch.h"
#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones)
{
    m_iNumChannels = pAnimation.iNumChannels;
    m_fDuration = pAnimation.fDuration;
    m_fTickPerSecond = pAnimation.fTickPerSecond;

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        CChannel* pChannel = CChannel::Create(pAnimation.Channels[i], Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

    return S_OK;
}

void CAnimation::Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, _bool isLoop, _bool* pFinished)
{
    //현재 키프레임
    m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

    //현재 트랙 포지션이 마지막에 도달했는지 체크
    if (m_fCurrentTrackPosition >= m_fDuration)
    {
        if (false == isLoop)
        {
            //루프가 아니면 끝났다고 알려줌
            *pFinished = true;
            m_fCurrentTrackPosition = m_fDuration;
            return;
        }
        else //루프면 0부터 다시 시작
            m_fCurrentTrackPosition = 0.f;

    }
    for (auto& pChannel : m_Channels)
    {
        //움직여야 될 뼈들에 키프레임을 전달
        pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition);
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

void CAnimation::Free()
{
    __super::Free();

    for (auto& pChannel : m_Channels)
        Safe_Release(pChannel);

    m_Channels.clear();
}
