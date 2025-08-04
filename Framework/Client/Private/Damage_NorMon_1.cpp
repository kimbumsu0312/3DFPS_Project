#include "pch.h"
#include "Damage_NorMon_1.h"

CDamage_NorMon_1::CDamage_NorMon_1()
{
}

HRESULT CDamage_NorMon_1::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CDamage_NorMon_1::Enter(const NORMON_STATE& pMonState)
{
    *m_pAnimState = ENUM_CLASS(NORMAL_MON_STATE::DAMAGE);
    m_eAnimState = STATE_ANIM::START;
    *m_pAnimTag = "Damage_M_F";
    *m_pIsAnimLoop = false;
}

void CDamage_NorMon_1::Update(_float fDeltatime, const NORMON_STATE& pMonState)
{
    if (*m_pIsAnimFinsh)
    {
        *m_pStateTag = TEXT("Chase");
    }
}

void CDamage_NorMon_1::Exit()
{
    m_eAnimState == STATE_ANIM::END;
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
