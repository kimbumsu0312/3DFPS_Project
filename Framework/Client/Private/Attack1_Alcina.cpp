#include "pch.h"
#include "Attack1_Alcina.h"

CAttack1_Alcina::CAttack1_Alcina()
{
}

HRESULT CAttack1_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack1_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    DIRECTION eDir = PlayerDIR(pContainer->Get_TransForm()->Get_State(STATE::POSITION), pContainer->Get_TransForm()->Get_State(STATE::LOOK));
    
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::ATTACK);
    if (eDir == DIRECTION::B || eDir == DIRECTION::BL || eDir == DIRECTION::BR)
    {
        pContainer->Switch_Anim("Claw_Behind_Start", false);
        m_szAttackName = TEXT("Claw_Behind_Start");
    }
    else if (m_iAttackCount == 0)
    {
        pContainer->Switch_Anim("Claw_Left_Zero_Start", false);
        m_iAttackCount++;
        m_szAttackName = TEXT("Claw_Left_Zero_Start");
    }
    else if(m_iAttackCount == 1)
    {
        pContainer->Switch_Anim("Claw_Left_Short_Start", false);
        m_iAttackCount++;
        m_szAttackName = TEXT("Claw_Left_Short_Start");
    }
    else if (m_iAttackCount == 2)
    {
        pContainer->Switch_Anim("Claw_Right_Short_Start", false);
        m_iAttackCount = 0;
        m_szAttackName = TEXT("Claw_Right_Short_Start");
    }
}

void CAttack1_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
            if (m_szAttackName == TEXT("Claw_Behind_Start"))
            {
                m_bisRight = false;
                pContainer->Trail_On(false);
                pContainer->Switch_Anim("Claw_Behind_Attack", false);
                m_szAttackName = TEXT("Claw_Behind_Attack");
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_S.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);

            }
            else if (m_szAttackName == TEXT("Claw_Left_Zero_Start"))
            {
                m_bisRight = false;
                pContainer->Trail_On(false);
                pContainer->Switch_Anim("Claw_Left_Zero_Attack", false);
                m_szAttackName = TEXT("Claw_Left_Zero_Attack");
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_S.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);
            }
            else if (m_szAttackName == TEXT("Claw_Left_Short_Start"))
            {
                m_bisRight = false;
                pContainer->Trail_On(false);
                pContainer->Switch_Anim("Claw_Left_Short_Attack", false);
                m_szAttackName = TEXT("Claw_Left_Short_Attack");

                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_S2.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);
            }
            else if (m_szAttackName == TEXT("Claw_Right_Short_Start"))
            {
                m_bisRight = true;
                pContainer->Trail_On(true);
                pContainer->Switch_Anim("Claw_Right_Short_Attack", false);
                m_szAttackName = TEXT("Claw_Right_Short_Attack");

                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_S2.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);


            }
 
            m_eAnimState = STATE_ANIM::LOOP2;
        }        
    }
    else if (m_eAnimState == STATE_ANIM::LOOP2)
    {
        pContainer->Attack_Collision(m_bisRight);
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            if (m_szAttackName == TEXT("Claw_Behind_Attack"))
            {
                pContainer->Switch_Anim("Claw_Behind_End", false);
                m_szAttackName = TEXT("Claw_Behind_End");
            }
            else if (m_szAttackName == TEXT("Claw_Left_Zero_Attack"))
            {
                pContainer->Switch_Anim("Claw_Left_Zero_End", false);
                m_szAttackName = TEXT("Claw_Left_Zero_End");
            }
            else if (m_szAttackName == TEXT("Claw_Left_Short_Attack"))
            {
                pContainer->Switch_Anim("Claw_Left_Short_End", false);
                m_szAttackName = TEXT("Claw_Left_Short_End");
            }
            else if (m_szAttackName == TEXT("Claw_Right_Short_Attack"))
            {
                pContainer->Switch_Anim("Claw_Right_Short_End", false);
                m_szAttackName = TEXT("Claw_Right_Short_End");

            }
            m_eAnimState = STATE_ANIM::END;
            pContainer->Trail_Off();
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
    }
}

void CAttack1_Alcina::Exit(CAlcina* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;

}

CAttack1_Alcina* CAttack1_Alcina::Create(void* pArg)
{
    CAttack1_Alcina* pInstance = new CAttack1_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack1_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack1_Alcina::Free()
{
    __super::Free();
}
