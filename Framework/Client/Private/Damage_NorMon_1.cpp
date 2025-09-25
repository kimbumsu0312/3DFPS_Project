#include "pch.h"
#include "Damage_NorMon_1.h"
#include "Monster_Normal.h"
#include "Player_Manager.h"
CDamage_NorMon_1::CDamage_NorMon_1()
{
}

HRESULT CDamage_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CDamage_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::DAMAGE);

    DIRECTION eHitPoint = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));

    _bool bHeadShot = pContainer->Get_BlackBoard()->Get_Data().bIsHeadShot;

    if(eHitPoint == DIRECTION::F || eHitPoint == DIRECTION::FR || eHitPoint == DIRECTION::FL)
        bHeadShot == true ? pContainer->Switch_Anim("Damage_HeadShot_F", false) : pContainer->Switch_Anim("Damage_M_F", false);
    else if(eHitPoint == DIRECTION::B)
        pContainer->Switch_Anim("Damage_M_B", false);
    else if ( eHitPoint == DIRECTION::R || eHitPoint == DIRECTION::BR)
        bHeadShot == true ? pContainer->Switch_Anim("Damage_HeadShot_R", false) : pContainer->Switch_Anim("Damage_M_L", false);
    else
        bHeadShot == true ? pContainer->Switch_Anim("Damage_HeadShot_L", false) : pContainer->Switch_Anim("Damage_M_RB", false);

    pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
    pContainer->Get_BlackBoard()->Set_Data().IsIdle = false;
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;

}

void CDamage_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
    {
        m_eAnimState = STATE_ANIM::LOOP;
        pContainer->Get_BlackBoard()->Set_Data().iDamage = 0;
        pContainer->Get_BlackBoard()->Set_Data().IsWeaponDamage = false;
        pContainer->Get_BlackBoard()->Set_Data().fAttackCool = 1.2f;

    }
}

void CDamage_NorMon_1::Exit(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}

CDamage_NorMon_1* CDamage_NorMon_1::Create(void* pArg)
{
    CDamage_NorMon_1* pInstance = new CDamage_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDamage_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDamage_NorMon_1::Free()
{
    __super::Free();
}
