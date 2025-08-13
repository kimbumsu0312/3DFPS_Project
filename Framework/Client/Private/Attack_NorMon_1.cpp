#include "pch.h"
#include "Attack_NorMon_1.h"

CAttack_NorMon_1::CAttack_NorMon_1()
{
}

HRESULT CAttack_NorMon_1::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}
void CAttack_NorMon_1::Enter(const NORMON_STATE& pMonState, CTransform* pTransformCom)
{
    _int Rand = rand() % 3;
    *m_pAnimState = ENUM_CLASS(NORMAL_MON_STATE::ATTACK);
    m_eAnimState = STATE_ANIM::START;
    if (pMonState.iWeponType == ENUM_CLASS(NORMAL_MON_WEAPON::END))
    {
        switch (Rand)
        {
        case 0:
            *m_pAnimTag = "Grapple_Attack_1";
            break;
        case 1:
            *m_pAnimTag = "Grapple_Attack_2";
            break;
        case 2:
            *m_pAnimTag = "Grapple_Attack_3";
            break;
        }
    }
    else
    {
        switch (Rand)
        {
        case 0:
            *m_pAnimTag = "Sword_Attack_1";
            break;
        case 1:
            *m_pAnimTag = "Sword_Attack_2";
            break;
        case 2:
            *m_pAnimTag = "Sword_Attack_3";
            break;
        }
    }
    *m_pIsAnimLoop = false;
}

void CAttack_NorMon_1::Update(_float fDeltatime, const NORMON_STATE& pMonState, CTransform* pTransformCom)
{
    //pTransformCom->Go_Straight(fDeltatime);
    if (pMonState.isDamage)
    {
        *m_pStateTag = TEXT("Damage");
        return;
    }

    if (*m_pIsAnimFinsh)
    {
        if(pMonState.isChase)
            *m_pStateTag = TEXT("Chase");
        else
            *m_pStateTag = TEXT("Scouting");
    }
}

void CAttack_NorMon_1::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

CAttack_NorMon_1* CAttack_NorMon_1::Create(void* pArg)
{
    CAttack_NorMon_1* pInstance = new CAttack_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_NorMon_1::Free()
{
    __super::Free();
}
