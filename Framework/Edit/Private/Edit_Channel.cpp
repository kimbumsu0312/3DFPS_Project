#include "pch.h"
#include "Edit_Channel.h"
#include "Edit_Bone.h"

CEdit_Channel::CEdit_Channel()
{
}

HRESULT CEdit_Channel::Initialize(const aiNodeAnim* pAIChannel, const vector<class CEdit_Bone*>& Bones, SAVE_ANIM* pAnim)
{
    SAVE_CHANNEL Channel;

    //현재 애니메이션 동작의 뼈와 이름이 일치하는 뼈를 찾고 인덱스를 가져온다.
    auto iter = find_if(Bones.begin(), Bones.end(), [&](CEdit_Bone* pBone)->_bool
    {
        if (true == pBone->Compare_Name(pAIChannel->mNodeName.data))
        {
            return true;
        }
        //이름이 일치하지 않으면 인덱스 ++
        m_iBoneIndex++;

        return false;
    });

    Channel.iBoneIndex = m_iBoneIndex;

    //키가 제일 많은 인덱스를 찾는다.
    m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
    m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);
    Channel.iNumKeyFrames = m_iNumKeyFrames;
    _float3 vScale{};
    _float4 vRotation{};
    _float3 vTranslation{};

    for (_int i = 0; i < m_iNumKeyFrames; ++i)
    {
        KEYFRAME KeyFrame{};

        //각 키가 i보다 클때만 복사
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

        //복사한 키를 keyFrame에 담고 넣어줌
        //키가 없어도 다른 키가 있을 경우, 현재 행렬을 유지해야하기 때문
        KeyFrame.vScale = vScale;
        KeyFrame.vRotation = vRotation;
        KeyFrame.vTranslation = vTranslation;

        //만든 키 프레임을 보관
        m_KeyFrames.push_back(KeyFrame);
        Channel.KeyFrames.push_back(KeyFrame);
    }
    pAnim->Channels.push_back(Channel);
    return S_OK;
}

void CEdit_Channel::Update_TransformationMatrix(const vector<class CEdit_Bone*>& Bones, _float fCurrentTrackPosition)
{
    _float fCurrentTrackPos = fCurrentTrackPosition;

    //현재 뼈들의 행렬을 구해온다.
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
