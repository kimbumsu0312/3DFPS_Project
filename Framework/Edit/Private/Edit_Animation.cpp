#include "pch.h"
#include "Edit_Animation.h"
#include "Edit_Channel.h"

CEdit_Animation::CEdit_Animation()
{
}

HRESULT CEdit_Animation::Initialize(const aiAnimation* pAIAnimation, const vector<class CEdit_Bone*>& Bones, SAVE_MODEL* pModelData)
{
    SAVE_ANIM Anim;

    //�ִϸ��̼��� ä��(��) ������ ������ �´�.
    Anim.iNumChannels = m_iNumChannels = pAIAnimation->mNumChannels;
    Anim.fDuration = m_fDuration = pAIAnimation->mDuration;
    Anim.fTickPerSecond = m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
    
    for (_int i = 0; i < m_iNumChannels; ++i)
    {
        //���� �ִϸ��̼ǿ� ä��(��)���� �����Ѵ�.
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
