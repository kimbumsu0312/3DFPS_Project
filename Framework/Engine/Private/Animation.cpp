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
    //���� Ű������
    m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

    //���� Ʈ�� �������� �������� �����ߴ��� üũ
    if (m_fCurrentTrackPosition >= m_fDuration)
    {
        if (false == isLoop)
        {
            //������ �ƴϸ� �����ٰ� �˷���
            *pFinished = true;
            m_fCurrentTrackPosition = m_fDuration;
            return;
        }
        else //������ 0���� �ٽ� ����
            m_fCurrentTrackPosition = 0.f;

    }
    for (auto& pChannel : m_Channels)
    {
        //�������� �� ���鿡 Ű�������� ����
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
