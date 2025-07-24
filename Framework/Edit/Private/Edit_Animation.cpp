#include "pch.h"
#include "Edit_Animation.h"
#include "Edit_Channel.h"

CEdit_Animation::CEdit_Animation()
{
}

HRESULT CEdit_Animation::Initialize(const aiAnimation* pAIAnimation, const vector<class CEdit_Bone*>& Bones, SAVE_MODEL* pModelData)
{
    SAVE_ANIM Anim;

    //애니메이션의 채널(뼈) 개수를 가지고 온다.
    Anim.iNumChannels = m_iNumChannels = pAIAnimation->mNumChannels;
    Anim.fDuration = m_fDuration = pAIAnimation->mDuration;
    Anim.fTickPerSecond = m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
    
    for (_int i = 0; i < m_iNumChannels; ++i)
    {
        //현재 애니메이션에 채널(뼈)들을 생성한다.
        CEdit_Channel* pChannel = CEdit_Channel::Create(pAIAnimation->mChannels[i], Bones, &Anim);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }
    pModelData->Animations.push_back(Anim);
	return S_OK;
}

void CEdit_Animation::Update_TransformationMatrices(const vector<class CEdit_Bone*>& Bones, _float fTimeDelta, _bool isLoop, _bool* pFinished)
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

CEdit_Animation* CEdit_Animation::Create(const aiAnimation* pAiAnimation, const vector<class CEdit_Bone*> Bones, SAVE_MODEL* pModelData)
{
    CEdit_Animation* pInstance = new CEdit_Animation();

    if (FAILED(pInstance->Initialize(pAiAnimation, Bones, pModelData)))
    {
        MSG_BOX(TEXT("Failed to Created : CAniCEdit_Animationmation"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEdit_Animation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
