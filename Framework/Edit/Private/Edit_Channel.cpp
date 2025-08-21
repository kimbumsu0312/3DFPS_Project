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

void CEdit_Channel::Update_TransformationMatrix(const vector<class CEdit_Bone*>& Bones, _float fCurrentTrackPosition, _float fPreTrackPosition, _uint* iCurrentKeyFrameIndex)
{
    //트랙 포지션 값을 0을 받으면 키 프레임 인덱스를 0으로 초기화 시킨다.
    if (fCurrentTrackPosition == 0.f)
        *iCurrentKeyFrameIndex = 0;

    //현재 뼈들의 행렬을 구해온다.
    _vector vScale, vRotation, vTranslation; 

    KEYFRAME        LastKeyFrame = m_KeyFrames.back();

    if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
    {
        //현재 받아온 트랙포지션이 마지막 키 프레임 트랙포지션보다 크거나 같으면 마지막 트랙 포지션 위치로 출력
        vScale = XMLoadFloat3(&LastKeyFrame.vScale);
        vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
    }
    else if (fPreTrackPosition > fCurrentTrackPosition)
    {
        if (fCurrentTrackPosition == 0.f || fPreTrackPosition > fCurrentTrackPosition)
            *iCurrentKeyFrameIndex = 0;

        while (*iCurrentKeyFrameIndex + 1 < m_KeyFrames.size() && fCurrentTrackPosition >= m_KeyFrames[*iCurrentKeyFrameIndex + 1].fTrackPosition)
        {
            ++*iCurrentKeyFrameIndex;
        }
        vScale = XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex].vScale);
        vRotation = XMLoadFloat4(&m_KeyFrames[*iCurrentKeyFrameIndex].vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex].vTranslation), 1.f);
    }
    else
    {
        ////현재 트랙 포지션이 현재 키프레임 트랙포지션 +1 이상이면 현재 키프레임 트랙 포지션 값을 올려준다.
        while (fCurrentTrackPosition >= m_KeyFrames[*iCurrentKeyFrameIndex + 1].fTrackPosition)
            ++*iCurrentKeyFrameIndex;
    
        ////현재 트랙 포지션 - 현재 키프레임의 트래 포지션 / 다음 키프레임의 트랙 포지션 - 다음 키프레임의 트랙 포지션
        _float fRatio = (fCurrentTrackPosition - m_KeyFrames[*iCurrentKeyFrameIndex].fTrackPosition) / (m_KeyFrames[*iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*iCurrentKeyFrameIndex].fTrackPosition);

        ////현재 키프레임에 각 값들을 fRatio 만큼 보간 시킨다.
        vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex + 1].vScale), fRatio);
        vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[*iCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[*iCurrentKeyFrameIndex + 1].vRotation), fRatio);
        vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);
    }

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vRotation, vTranslation);

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
