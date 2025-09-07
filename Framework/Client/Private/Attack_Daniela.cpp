#include "pch.h"
#include "Attack_Daniela.h"
#include "Daniela.h"

CAttack_Daniela::CAttack_Daniela()
{
}

HRESULT CAttack_Daniela::Initalize(void* pArg)
{
    m_iAttackType = 0;

    return S_OK;
}

void CAttack_Daniela::Enter(CDaniela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CDaniela::ANIM_STATE::ATTACK);

    DIRECTION ePlayerDir = PlayerDIR(pContainer->Get_Transform()->Get_State(STATE::POSITION), pContainer->Get_Transform()->Get_State(STATE::LOOK));

    if (ePlayerDir == DIRECTION::F || ePlayerDir == DIRECTION::FR || ePlayerDir == DIRECTION::FL)
    {
        switch (m_iAttackType)
        {
        case 0:
            pContainer->Switch_Anim("Attack_1", false);
            break;
        case 1:
            pContainer->Switch_Anim("Attack_2", false);
            break;
        case 2:
            pContainer->Switch_Anim("Attack_3", false);
            break;
        }
        ++m_iAttackType;

        if (m_iAttackType >= 3)
            m_iAttackType = 0;
    }
    else if(ePlayerDir == DIRECTION::L || ePlayerDir == DIRECTION::BL)
    {
        pContainer->Switch_Anim("Attack_Turn_L", false);
    }
    else if (ePlayerDir == DIRECTION::R || ePlayerDir == DIRECTION::BR || ePlayerDir == DIRECTION::B)
    {
        pContainer->Switch_Anim("Attack_Turn_R", false);
    }
}

void CAttack_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Target_LookTurn_Navi(fDeltatime);
        pContainer->Attack_Collision();
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().fAttackCool = 5.f;
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
        }
    }
}

void CAttack_Daniela::Exit(CDaniela* pContainer)
{
}

CAttack_Daniela* CAttack_Daniela::Create(void* pArg)
{
    CAttack_Daniela* pInstance = new CAttack_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_Daniela::Free()
{
    __super::Free();
}
