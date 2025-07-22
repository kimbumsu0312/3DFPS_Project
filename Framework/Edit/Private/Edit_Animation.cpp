#include "pch.h"
#include "Edit_Animation.h"
#include "Edit_Channel.h"

CEdit_Animation::CEdit_Animation()
{
}

HRESULT CEdit_Animation::Initialize(const aiAnimation* pAIAnimation, const vector<class CEdit_Bone*>& Bones)
{
    //애니메이션의 채널(뼈) 개수를 가지고 온다.
    m_iNumChannels = pAIAnimation->mNumChannels;

    for (_int i = 0; i < m_iNumChannels; ++i)
    {
        //현재 애니메이션에 채널(뼈)들을 생성한다.
        CEdit_Channel* pChannel = CEdit_Channel::Create(pAIAnimation->mChannels[i], Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }
    m_fTickPerSecond = 20.f;
	return S_OK;
}

void CEdit_Animation::Update_TransformationMatrices(const vector<class CEdit_Bone*>& Bones, _float fTimeDelta)
{
    //현재 키프레임
    m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

    for (auto& pChannel : m_Channels)
    {
        //움직여야 될 뼈들에 키프레임을 전달
        pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition);
    }
}

CEdit_Animation* CEdit_Animation::Create(const aiAnimation* pAiAnimation, const vector<class CEdit_Bone*> Bones)
{
    CEdit_Animation* pInstance = new CEdit_Animation();

    if (FAILED(pInstance->Initialize(pAiAnimation, Bones)))
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
