#include "pch.h"
#include "Stand_NorMon_1.h"
#include "Monster_Normal.h"

CStand_NorMon_1::CStand_NorMon_1()
{
}

HRESULT CStand_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CStand_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_AnimState(ENUM_CLASS(NORMAL_MON_STATE::NORMAL));
    pContainer->Switch_Anim("Idle_Loop", true);
}

void CStand_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    if (pContainer->Get_State().isDamage)
    {
        pContainer->Switch_State(TEXT("Damage"));
    }
    else if (pContainer->Get_State().isChase)
    {
        pContainer->Switch_State(TEXT("Chase"));
    }
}

void CStand_NorMon_1::Exit(CMonster_Normal* pContainer)
{
}

CStand_NorMon_1* CStand_NorMon_1::Create(void* pArg)
{
    CStand_NorMon_1* pInstance = new CStand_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CStand_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStand_NorMon_1::Free()
{
    __super::Free();
}
