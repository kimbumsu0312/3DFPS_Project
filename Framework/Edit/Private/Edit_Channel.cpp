#include "pch.h"
#include "Edit_Channel.h"
#include "Edit_Bone.h"

CEdit_Channel::CEdit_Channel()
{
}

HRESULT CEdit_Channel::Initialize(const aiNodeAnim* pAIChannel, const vector<class CEdit_Bone*>& Bones, SAVE_ANIM* pAnim)
{
    SAVE_CHANNEL Channel;

    //���� �ִϸ��̼� ������ ���� �̸��� ��ġ�ϴ� ���� ã�� �ε����� �����´�.
    auto iter = find_if(Bones.begin(), Bones.end(), [&](CEdit_Bone* pBone)->_bool
    {
        if (true == pBone->Compare_Name(pAIChannel->mNodeName.data))
        {
            return true;
        }
        //�̸��� ��ġ���� ������ �ε��� ++
        m_iBoneIndex++;

        return false;
    });

    Channel.iBoneIndex = m_iBoneIndex;

    //Ű�� ���� ���� �ε����� ã�´�.
    m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
    m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);
    Channel.iNumKeyFrames = m_iNumKeyFrames;
    _float3 vScale{};
    _float4 vRotation{};
    _float3 vTranslation{};

    for (_int i = 0; i < m_iNumKeyFrames; ++i)
    {
        KEYFRAME KeyFrame{};

        //�� Ű�� i���� Ŭ���� ����
        if (i < pAIChannel->mNumScalingKeys)
        {
            memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
            KeyFrame.fTrackPosition = pAIChannel->mScalingKeys[i].mTime;
        }

        if (i < pAIChannel->mNumRotationKeys)
        {
            vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
            vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
            vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
            vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;

            KeyFrame.fTrackPosition = pAIChannel->mRotationKeys[i].mTime;
        }

        if (i < pAIChannel->mNumPositionKeys)
        {
            memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
            KeyFrame.fTrackPosition = pAIChannel->mPositionKeys[i].mTime;
        }

        //������ Ű�� keyFrame�� ��� �־���
        //Ű�� ��� �ٸ� Ű�� ���� ���, ���� ����� �����ؾ��ϱ� ����
        KeyFrame.vScale = vScale;
        KeyFrame.vRotation = vRotation;
        KeyFrame.vTranslation = vTranslation;

        //���� Ű �������� ����
        m_KeyFrames.push_back(KeyFrame);
        Channel.KeyFrames.push_back(KeyFrame);
    }
    pAnim->Channels.push_back(Channel);
    return S_OK;
}

void CEdit_Channel::Update_TransformationMatrix(const vector<class CEdit_Bone*>& Bones, _float fCurrentTrackPosition)
{
    _float fCurrentTrackPos = fCurrentTrackPosition;

    //���� ������ ����� ���ؿ´�.
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

CEdit_Channel* CEdit_Channel::Create(const aiNodeAnim* pAIChannel, const vector<class CEdit_Bone*>& Bones, SAVE_ANIM* pAnim)
{
    CEdit_Channel* pInstance = new CEdit_Channel();

    if (FAILED(pInstance->Initialize(pAIChannel, Bones, pAnim)))
    {
        MSG_BOX(TEXT("Failed to Created :CEdit_Channel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEdit_Channel::Free()
{
    __super::Free();

    m_KeyFrames.clear();
}
