#include "pch.h"
#include "Scouting_NorMon_1.h"

CScouting_NorMon_1::CScouting_NorMon_1()
{
}

HRESULT CScouting_NorMon_1::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CScouting_NorMon_1::Enter(const NORMON_STATE& pMonState, CTransform* pTransformCom)
{
    *m_pAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
    m_eAnimState = STATE_ANIM::START;
    *m_pAnimTag = "Walk_Start";
    *m_pIsAnimLoop = false;
}

void CScouting_NorMon_1::Update(_float fDeltatime, const NORMON_STATE& pMonState, CTransform* pTransformCom)
{
    pTransformCom->Go_Straight(fDeltatime);
    
    if (pMonState.isDamage)
    {
        *m_pStateTag = TEXT("Damage");
    }
    if(*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
    { 
        m_eAnimState = STATE_ANIM::LOOP;
        *m_pAnimTag = "Walk_Loop";
        *m_pIsAnimLoop = true;
    }
    if(pMonState.isChase == true)
        *m_pStateTag = TEXT("Chase");
}
void CScouting_NorMon_1::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

CScouting_NorMon_1* CScouting_NorMon_1::Create(void* pArg)
{
    CScouting_NorMon_1* pInstance = new CScouting_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CScouting_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CScouting_NorMon_1::Free()
{
    __super::Free();
}
