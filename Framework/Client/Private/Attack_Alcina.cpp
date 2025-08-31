#include "pch.h"
#include "Attack_Alcina.h"

CAttack_Alcina::CAttack_Alcina()
{
}

HRESULT CAttack_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    CAlcina::Attack_Type eAttack = pContainer->Get_BlackBoard()->Get_Data().eAttackType;

    DIRECTION eDir = PlayerDIR(pContainer->Get_TransForm()->Get_State(STATE::POSITION), pContainer->Get_TransForm()->Get_State(STATE::LOOK));
    
    pContainer->Switch_AnimState(ENUM_CLASS(BOSS_SISTER::ATTACK));
    if (eDir == DIRECTION::B || eDir == DIRECTION::BL || eDir == DIRECTION::BR)
        pContainer->Switch_Anim("Claw_Behind", false);
    else if (eAttack == CAlcina::Attack_Type::LONG)
        pContainer->Switch_Anim("Claw_Moving_Attack_Start", false);
    else if (eAttack == CAlcina::Attack_Type::SHORT)
        pContainer->Switch_Anim("Claw_Left_Short", false);
    else if (eAttack == CAlcina::Attack_Type::ZERO)
        pContainer->Switch_Anim("Claw_Left_Zero", false);

}

void CAttack_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    CAlcina::Attack_Type eAttack = pContainer->Get_BlackBoard()->Get_Data().eAttackType;
    
    if (eAttack == CAlcina::Attack_Type::LONG)
    {
        if (m_eAnimState == STATE_ANIM::START)
        {
            if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            {
                m_eAnimState = STATE_ANIM::LOOP;
                pContainer->Switch_Anim("Claw_Moving_Attack_Loop", false);
            }
        }
        else if(m_eAnimState == STATE_ANIM::LOOP)
        {
            pContainer->Target_LookTurn(fDeltatime);
            if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            {  
                m_eAnimState = STATE_ANIM::END;
                pContainer->Switch_Anim("Claw_Moving_Attack_End", false);
            }
        }
        else if (m_eAnimState == STATE_ANIM::END)
        {
            if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
                *pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
        }
    }
    else
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            *pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
    }
}

void CAttack_Alcina::Exit(CAlcina* pContainer)
{
}

CAttack_Alcina* CAttack_Alcina::Create(void* pArg)
{
    CAttack_Alcina* pInstance = new CAttack_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_Alcina::Free()
{
    __super::Free();
}
