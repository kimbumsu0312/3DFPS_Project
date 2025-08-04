#include "pch.h"
#include "Stand_NorMon_1.h"

CStand_NorMon_1::CStand_NorMon_1()
{
}

HRESULT CStand_NorMon_1::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CStand_NorMon_1::Enter(const NORMON_STATE& pMonState)
{
    *m_pAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
    m_eAnimState = STATE_ANIM::LOOP;
    *m_pAnimTag = "Idle_Loop";
    *m_pIsAnimLoop = true;
}

void CStand_NorMon_1::Update(_float fDeltatime, const NORMON_STATE& pMonState)
{
    if (pMonState.isDamage)
    {
        *m_pStateTag = TEXT("Damage");
    }
    else if (pMonState.isChase)
    {
        *m_pStateTag = TEXT("Chase");
    }
}

void CStand_NorMon_1::Exit()
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
