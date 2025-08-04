#include "pch.h"
#include "Chase_NorMon_1.h"

CChase_NorMon_1::CChase_NorMon_1()
{
}

HRESULT CChase_NorMon_1::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}
void CChase_NorMon_1::Enter(const NORMON_STATE& pMonState)
{
    *m_pAnimState = ENUM_CLASS(NORMAL_MON_STATE::ATTACK);
    m_eAnimState = STATE_ANIM::START;
    if(pMonState.iWeponType == ENUM_CLASS(NORMAL_MON_WEAPON::END))
        *m_pAnimTag = "Grapple_Walk_Start";
    else
        *m_pAnimTag = "Sword_Walk_Start";

    *m_pIsAnimLoop = false;
    if (pMonState.isAttack)
        m_eAnimState = STATE_ANIM::END;

}

void CChase_NorMon_1::Update(_float fDeltatime, const NORMON_STATE& pMonState)
{
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
        m_eAnimState = STATE_ANIM::LOOP;

    if (pMonState.isDamage)
    {
        *m_pStateTag = TEXT("Damage");
        return;
    }

    if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (pMonState.iWeponType == ENUM_CLASS(NORMAL_MON_WEAPON::END))
            *m_pAnimTag = "Grapple_Walk_Loop";
        else
            *m_pAnimTag = "Sword_Walk_Loop";
        
        *m_pIsAnimLoop = true;
        
    }

    if (m_eAnimState == STATE_ANIM::END)
    {
        if (*m_pIsAnimFinsh)
        {
            *m_pStateTag = TEXT("Attack");
        }

        if (pMonState.iWeponType == ENUM_CLASS(NORMAL_MON_WEAPON::END))
            *m_pAnimTag = "Grapple_Walk_End";
        else
            *m_pAnimTag = "Sword_Walk_End";

        *m_pIsAnimLoop = false;
    }

}

void CChase_NorMon_1::Exit()
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
