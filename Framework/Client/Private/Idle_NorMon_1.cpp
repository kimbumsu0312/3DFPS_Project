#include "pch.h"
#include "Idle_NorMon_1.h"
#include "Monster_Normal.h"

CIdle_NorMon_1::CIdle_NorMon_1()
{
}

HRESULT CIdle_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CIdle_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);

    m_iStartType = pContainer->Get_BlackBoard()->Get_Data().iStartMotion;
      
    switch (m_iStartType)
    {
    case  ENUM_CLASS(IDLE_TYPE::STAND):
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
        pContainer->Switch_Anim("Stand_Loop", true);
        break;
    case  ENUM_CLASS(IDLE_TYPE::SIT):
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
        pContainer->Switch_Anim("Sit_Loop", true);
        break;
    case  ENUM_CLASS(IDLE_TYPE::CLIME):
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
        pContainer->Switch_Anim("Clime_Pop", false);
        m_eAnimState = STATE_ANIM::LOOP;
        break;
    case  ENUM_CLASS(IDLE_TYPE::FALL):
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::FALL);
        pContainer->Switch_Anim("Fall_Start", false);
        m_eAnimState = STATE_ANIM::LOOP;
        m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_1));
        m_pGameInstance->PlaySoundW(TEXT("Mon_fall.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_1), g_fBGMVolume);
        break;
    }

}

void CIdle_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        if (pContainer->Get_BlackBoard()->Get_Data().IsChase == true)
        {
            m_eAnimState = STATE_ANIM::LOOP;
        }
    }

    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        switch (m_iStartType)
        {
        case  ENUM_CLASS(IDLE_TYPE::STAND):
            pContainer->Switch_Anim("Stand_End", false);
            m_eAnimState = STATE_ANIM::END;
            break;
        case  ENUM_CLASS(IDLE_TYPE::SIT):
            pContainer->Switch_Anim("Sit_End", false);
            m_eAnimState = STATE_ANIM::END;
            break;
        case  ENUM_CLASS(IDLE_TYPE::CLIME):
            if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            {
                *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
                pContainer->Get_BlackBoard()->Set_Data().IsIdle = false;
                pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
            }
            break;
        case  ENUM_CLASS(IDLE_TYPE::FALL):
            if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            {
                *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
                pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
                pContainer->Get_BlackBoard()->Set_Data().IsIdle = false;
                pContainer->Switch_Anim("Idle_Loop", true);


            }
            break;
        }
    }
    else
    {
        switch (m_iStartType)
        {
        case  ENUM_CLASS(IDLE_TYPE::STAND):
            break;
        case  ENUM_CLASS(IDLE_TYPE::SIT):
            pContainer->Target_LookAt(fDeltatime);
            break;
        case  ENUM_CLASS(IDLE_TYPE::CLIME):
            break;
        case  ENUM_CLASS(IDLE_TYPE::FALL):
            break;
        }

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
            pContainer->Switch_Anim("Idle_Loop", true);
            pContainer->Get_BlackBoard()->Set_Data().IsIdle = false;
            pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
 
        }
    }

}

void CIdle_NorMon_1::Exit(CMonster_Normal* pContainer)
{
}

CIdle_NorMon_1* CIdle_NorMon_1::Create(void* pArg)
{
    CIdle_NorMon_1* pInstance = new CIdle_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_NorMon_1::Free()
{
    __super::Free();
}
