#include "pch.h"
#include "Damage_Werewolf.h"
#include "Monster_WereWolf.h"

CDamage_WereWolf::CDamage_WereWolf()
{
}

HRESULT CDamage_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CDamage_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::DAMAGE);

    DIRECTION eHitPoint = PlayerDIR(pContainer->Get_Transform()->Get_State(STATE::POSITION), pContainer->Get_Transform()->Get_State(STATE::LOOK));

    
    if (eHitPoint == DIRECTION::F || eHitPoint == DIRECTION::FR || eHitPoint == DIRECTION::FL)
        pContainer->Switch_Anim("Damage_Large_F", false);
    else if (eHitPoint == DIRECTION::B)
        pContainer->Switch_Anim("Damage_Large_B", false);
    else if (eHitPoint == DIRECTION::R || eHitPoint == DIRECTION::BR)
        pContainer->Switch_Anim("Damage_Large_R", false);
    else
        pContainer->Switch_Anim("Damage_Large_L", false);
}

void CDamage_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
    {
        m_eAnimState = STATE_ANIM::LOOP;
        pContainer->Get_BlackBoard()->Set_Data().iDamage = 0;
    }
}

void CDamage_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CDamage_WereWolf* CDamage_WereWolf::Create(void* pArg)
{
    CDamage_WereWolf* pInstance = new CDamage_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDamage_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDamage_WereWolf::Free()
{
    __super::Free();
}
