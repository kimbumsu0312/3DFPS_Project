#include "pch.h"
#include "Attack_NorMon_1.h"
#include "Monster_Normal.h"

CAttack_NorMon_1::CAttack_NorMon_1()
{
}

HRESULT CAttack_NorMon_1::Initalize(void* pArg)
{
    m_iAttackType = 0;
    return S_OK;
}

void CAttack_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::ATTACK);
    
    if (pContainer->Get_BlackBoard()->Get_Data().iWeapon == ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END))
    {
        switch (m_iAttackType)
        {
        case 0:
            pContainer->Switch_Anim("Grapple_Attack_1_Start", false);
            break;
        case 1:
            pContainer->Switch_Anim("Grapple_Attack_2_Start", false);
            break;
        case 2:
            pContainer->Switch_Anim("Grapple_Attack_3_Start", false);
            break;
        }
    }
    else
    {
        switch (m_iAttackType)
        {
        case 0:
            pContainer->Switch_Anim("Sword_Attack_1_Start", false);
            break;
        case 1:
            pContainer->Switch_Anim("Sword_Attack_2_Start", false);
            break;
        case 2:
            pContainer->Switch_Anim("Sword_Attack_3_Start", false);
            break;
        }
    }

}

void CAttack_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    m_fAccTime += fDeltatime;
    if (m_fAccTime > 1.f)
    {
        m_fAccTime = 0.f;
        m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_1_MOVE));
        m_pGameInstance->PlaySoundW(TEXT("Normon_1_Move.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_1_MOVE), g_fBGMVolume);

    }

    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if(m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Target_LookAt(fDeltatime);
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            ++m_iAttackType;
            m_eAnimState = STATE_ANIM::END;
            pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
            if (pContainer->Get_BlackBoard()->Get_Data().iWeapon == ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END))
            {
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_1));
                m_pGameInstance->PlaySoundW(TEXT("Normon_1_Attack_2.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_1), g_fBGMVolume);

                switch (m_iAttackType)
                {
                case 0:
                    pContainer->Switch_Anim("Grapple_Attack_1", false);
                    break;
                case 1:
                    pContainer->Switch_Anim("Grapple_Attack_2", false);
                    break;
                case 2:
                    pContainer->Switch_Anim("Grapple_Attack_3", false);
                    break;
                }
            }
            else
            {
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_1));
                m_pGameInstance->PlaySoundW(TEXT("Normon_1_Attack.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_1), g_fBGMVolume);

                switch (m_iAttackType)
                {
                case 0:
                    pContainer->Switch_Anim("Sword_Attack_1", false);
                    break;
                case 1:
                    pContainer->Switch_Anim("Sword_Attack_2", false);
                    break;
                case 2:
                    pContainer->Switch_Anim("Sword_Attack_3", false);
                    break;
                }
            }
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        pContainer->Target_LookAt(fDeltatime);
        pContainer->Attack_Collision();
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = true;
            pContainer->Get_BlackBoard()->Set_Data().fAttackCool = 2.f;
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;

            if (m_iAttackType >= 3)
                m_iAttackType = 0;
        }
    }
}

void CAttack_NorMon_1::Exit(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}

CAttack_NorMon_1* CAttack_NorMon_1::Create(void* pArg)
{
    CAttack_NorMon_1* pInstance = new CAttack_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_NorMon_1::Free()
{
    __super::Free();
}
