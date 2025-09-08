#include "pch.h"
#include "Attack_Werewolf.h"
#include "Monster_WereWolf.h"

CAttack_WereWolf::CAttack_WereWolf()
{
}

HRESULT CAttack_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::ATTACK);

    DIRECTION ePlayerDir = PlayerDIR(pContainer->Get_Transform()->Get_State(STATE::POSITION), pContainer->Get_Transform()->Get_State(STATE::LOOK));

    if (ePlayerDir == DIRECTION::F || ePlayerDir == DIRECTION::FR || ePlayerDir == DIRECTION::FL)
    {
        pContainer->Switch_Anim("Attack_F", false);
    }
    else if (ePlayerDir == DIRECTION::L || ePlayerDir == DIRECTION::BL)
    {
        pContainer->Switch_Anim("Attack_L", false);
    }
    else if (ePlayerDir == DIRECTION::R || ePlayerDir == DIRECTION::BR || ePlayerDir == DIRECTION::B)
    {
        pContainer->Switch_Anim("Attack_R", false);
    }
}

void CAttack_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Target_LookTurn(fDeltatime);
        pContainer->Attack_Collision();

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().fAttackCool = 5.f;
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
        }
    }
}

void CAttack_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CAttack_WereWolf* CAttack_WereWolf::Create(void* pArg)
{
    CAttack_WereWolf* pInstance = new CAttack_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_WereWolf::Free()
{
    __super::Free();
}
