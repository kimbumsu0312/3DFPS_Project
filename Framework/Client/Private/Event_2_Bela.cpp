#include "pch.h"
#include "Event_2_Bela.h"
#include "Bela.h"
#include "Player_Manager.h"

CEvent_2_Bela::CEvent_2_Bela()
{
}

HRESULT CEvent_2_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CEvent_2_Bela::Enter(CBela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CBela::ANIM_STATE::ATTACK);
    pContainer->Switch_Anim("Freeszes_Swing_Start", false);

    _matrix vPlayerMat = CPlayer_Manager::GetInstance()->Get_PlayerWorld();
    _vector vScale, vWorldRot, vWorldTrans;
    XMMatrixDecompose(&vScale, &vWorldRot, &vWorldTrans, vPlayerMat);

    _vector vLook = { XMVectorGetX(vPlayerMat.r[2]), XMVectorGetY(vPlayerMat.r[2]), XMVectorGetZ(vPlayerMat.r[2]), 0.f };

    _vector vOffset = { 0.f, 0.f , 3.f, 1.f };
    vOffset = XMVector3Rotate(vOffset, vWorldRot);
    _vector vPos = vWorldTrans + vOffset;

    pContainer->Get_Transform()->Set_State(STATE::POSITION, vPos);
    pContainer->Get_Navi()->Set_CellIndex(CPlayer_Manager::GetInstance()->Get_CellIndex());
    pContainer->Get_Transform()->LookAt(vWorldTrans);

    pContainer->Get_BlackBoard()->Set_Data().fNoies = 1.f;
    m_fAccTime = 0.f;

    pContainer->Get_BlackBoard()->Set_Data().bIsFly = false;
    pContainer->Spawn_EffectReset();
    pContainer->Get_BlackBoard()->Set_Data().bIsSpawnFly = true;

}

void CEvent_2_Bela::Update(CBela* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        m_fAccTime += fDeltatime * 0.02f;
        m_fEffectTime += fDeltatime;

        pContainer->Get_BlackBoard()->Set_Data().bIsFly = true;

        pContainer->Get_BlackBoard()->Set_Data().fNoies -= m_fAccTime;

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP2;
            pContainer->Switch_Anim("Freeszes_Swing1", false);

            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::BELA));
            m_pGameInstance->PlaySoundW(TEXT("Sister_Attack.wav"), ENUM_CLASS(SOUND_CHANNEL::BELA), g_fBGMVolume);
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP2)
    {
        pContainer->Attack_Collision();

        m_fAccTime += fDeltatime * 0.02f;
        m_fEffectTime += fDeltatime;

        pContainer->Get_BlackBoard()->Set_Data().fNoies -= m_fAccTime;

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP3;
            pContainer->Switch_Anim("Freeszes_Swing2", false);
            pContainer->Get_BlackBoard()->Set_Data().bIsSpawnFly = false;
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::BELA));
            m_pGameInstance->PlaySoundW(TEXT("Sister_Attack.wav"), ENUM_CLASS(SOUND_CHANNEL::BELA), g_fBGMVolume);

            pContainer->Get_BlackBoard()->Set_Data().fNoies = 0.f;
            m_fAccTime = 0.f;

        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP3)
    {
        pContainer->Attack_Collision();

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP4;
            pContainer->Switch_Anim("Freeszes_Swing3", true);

            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::BELA));
            m_pGameInstance->PlaySoundW(TEXT("Sister_Attack.wav"), ENUM_CLASS(SOUND_CHANNEL::BELA), g_fBGMVolume);

        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP4)
    {
        pContainer->Attack_Collision();

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP5;
            pContainer->Switch_Anim("Freeszes_Swing4", true);

            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::BELA));
            m_pGameInstance->PlaySoundW(TEXT("Sister_Attack.wav"), ENUM_CLASS(SOUND_CHANNEL::BELA), g_fBGMVolume);
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP5)
    {
        pContainer->Attack_Collision();

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::END;
            *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CBela::ANIM_STATE::NORMAL);
            pContainer->Switch_Anim("Idle_Freezes", true);

            pContainer->Get_BlackBoard()->Set_Data().fNoies = 0.f;
            m_fAccTime = 0.f;
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        pContainer->Target_LookTurn(fDeltatime);
        m_fAccTime += fDeltatime * 0.02f;
        pContainer->Get_BlackBoard()->Set_Data().fNoies += m_fAccTime;
        if (pContainer->Get_BlackBoard()->Set_Data().fNoies >= 1.f)
        {
            pContainer->Get_BlackBoard()->Set_Data().IsEvent_2 = false;
            pContainer->Get_BlackBoard()->Set_Data().fMoveSpeed = 3.f;
        }
    }
}

void CEvent_2_Bela::Exit(CBela* pContainer)
{
}

CEvent_2_Bela* CEvent_2_Bela::Create(void* pArg)
{
    CEvent_2_Bela* pInstance = new CEvent_2_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CEvent_2_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEvent_2_Bela::Free()
{
    __super::Free();
}
