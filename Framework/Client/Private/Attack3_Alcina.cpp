#include "pch.h"
#include "Attack3_Alcina.h"

CAttack3_Alcina::CAttack3_Alcina()
{
}

HRESULT CAttack3_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack3_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::ATTACK);
    pContainer->Get_BlackBoard()->Set_Data().fNoies = 1.f;
    if (m_iAttackCount == 0)
    {
        pContainer->Switch_Anim("Claw_Diagonal_Start", false);
        m_szAttackName = TEXT("Claw_Diagonal_Start");
        m_iAttackCount++;
    }
    else if (m_iAttackCount == 1)
    {
        pContainer->Switch_Anim("Claw_Vertical_Start", false);
        m_szAttackName = TEXT("Claw_Vertical_Start");
        m_iAttackCount++;
    }
    else if (m_iAttackCount == 2)
    {
        pContainer->Switch_Anim("Claw_Distance_Start", false);
        m_szAttackName = TEXT("Claw_Distance_Start");
        m_iAttackCount = 0;
    }

}

void CAttack3_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{

    pContainer->Target_LookTurn(fDeltatime * 2.f);
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies -= fDeltatime;
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
            if (m_szAttackName == TEXT("Claw_Diagonal_Start"))
            {
                pContainer->Trail_On(true);
                pContainer->Switch_Anim("Claw_Diagonal_Attack", false);
                m_szAttackName = TEXT("Claw_Diagonal_Attack");

                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_M.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);
            }
            else if (m_szAttackName == TEXT("Claw_Vertical_Start"))
            {
                pContainer->Trail_On(true);
                pContainer->Switch_Anim("Claw_Vertical_Attack", false);
                m_szAttackName = TEXT("Claw_Vertical_Attack");

                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_M.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);
            }
            else if (m_szAttackName == TEXT("Claw_Distance_Start"))
            {
                pContainer->Trail_On(true);
                pContainer->Switch_Anim("Claw_Distance_Attack", false);
                m_szAttackName = TEXT("Claw_Distance_Attack");

                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
                m_pGameInstance->PlaySoundW(TEXT("Alcina_Attack_L.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume);

            }

            m_eAnimState = STATE_ANIM::LOOP2;
        }

    }
    else if (m_eAnimState == STATE_ANIM::LOOP2)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies -= fDeltatime;
        pContainer->Attack_Collision(true);
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            if (m_szAttackName == TEXT("Claw_Diagonal_Attack"))
            {
                pContainer->Switch_Anim("Claw_Diagonal_End", false);
                m_szAttackName = TEXT("Claw_Diagonal_End");
            }
            else if (m_szAttackName == TEXT("Claw_Vertical_Attack"))
            {
                pContainer->Switch_Anim("Claw_Vertical_End", false);
                m_szAttackName = TEXT("Claw_Vertical_End");
            }
            else if (m_szAttackName == TEXT("Claw_Distance_Attack"))
            {
                pContainer->Switch_Anim("Claw_Distance_End", false);
                m_szAttackName = TEXT("Claw_Distance_End");

            }
            pContainer->Get_BlackBoard()->Set_Data().fNoies = 0.f;
            m_eAnimState = STATE_ANIM::END;
            pContainer->Trail_Off();
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime * 0.5f;
        
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
    }
}

void CAttack3_Alcina::Exit(CAlcina* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;
}

CAttack3_Alcina* CAttack3_Alcina::Create(void* pArg)
{
    CAttack3_Alcina* pInstance = new CAttack3_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack3_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack3_Alcina::Free()
{
    __super::Free();
}
