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
    pContainer->Switch_AnimState(ENUM_CLASS(NORMAL_MON_STATE::DAMAGE));
    DIRECTION eHitPoint = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));

    if(eHitPoint == DIRECTION::F || eHitPoint == DIRECTION::FR || eHitPoint == DIRECTION::FL)
        (pContainer->IsHeadShot()) ? pContainer->Switch_Anim("Damage_HeadShot_F", false) : pContainer->Switch_Anim("Damage_M_F", false);
    else if(eHitPoint == DIRECTION::B)
        pContainer->Switch_Anim("Damage_M_B", false);
    else if ( eHitPoint == DIRECTION::R || eHitPoint == DIRECTION::BR)
        (pContainer->IsHeadShot()) ? pContainer->Switch_Anim("Damage_HeadShot_R", false) : pContainer->Switch_Anim("Damage_M_L", false);
    else
        (pContainer->IsHeadShot()) ? pContainer->Switch_Anim("Damage_HeadShot_L", false) : pContainer->Switch_Anim("Damage_M_RB", false);

}

void CDamage_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    if (pContainer->IsAnimFinsh())
    {
        m_eAnimState = STATE_ANIM::LOOP;
        pContainer->Switch_State(TEXT("Chase"));
    }
}

void CDamage_NorMon_1::Exit(CMonster_Normal* pContainer)
{
    m_eAnimState == STATE_ANIM::END;
    pContainer->Reset_DamageCheck();
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
