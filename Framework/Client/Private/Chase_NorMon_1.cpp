#include "pch.h"
#include "Chase_NorMon_1.h"
#include "Player_Manager.h"
#include "Monster_Normal.h"

CChase_NorMon_1::CChase_NorMon_1()
{
}

HRESULT CChase_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CChase_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    pContainer->Switch_AnimState(ENUM_CLASS(NORMAL_MON_STATE::ATTACK));
    
    if (pContainer->Get_WeaponType() == ENUM_CLASS(NORMAL_MON_WEAPON::END))
        pContainer->Switch_Anim("Grapple_Walk_Start", false);
    else
        pContainer->Switch_Anim("Sword_Walk_Start", false);

}

void CChase_NorMon_1::Update(CMonster_Normal* pContainer, _float fTimeDelta)
{
    pContainer->Target_LookAt(fTimeDelta);
    if (pContainer->IsAnimFinsh() && m_eAnimState == STATE_ANIM::START)
        m_eAnimState = STATE_ANIM::LOOP;

    if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (pContainer->Get_WeaponType() == ENUM_CLASS(NORMAL_MON_WEAPON::END))
            pContainer->Switch_Anim("Grapple_Walk_Loop", true);
        else
            pContainer->Switch_Anim("Sword_Walk_Loop", true);

        if (pContainer->Get_State().isDamage)
            pContainer->Switch_State(TEXT("Damage"));
        else if (pContainer->Get_State().isAttack)
            pContainer->Switch_State(TEXT("Attack"));
    }
}

void CChase_NorMon_1::Exit(CMonster_Normal* pContainer)
{
}

CChase_NorMon_1* CChase_NorMon_1::Create(void* pArg)
{
    CChase_NorMon_1* pInstance = new CChase_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CChase_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChase_NorMon_1::Free()
{
    __super::Free();
}
