#include "pch.h"
#include "Scouting_NorMon_1.h"
#include "Monster_Normal.h"
CScouting_NorMon_1::CScouting_NorMon_1()
{
}

HRESULT CScouting_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CScouting_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
    pContainer->Switch_Anim("Walk_Start", false);
}

void CScouting_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true && m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
        pContainer->Switch_Anim("Walk_Loop", true);
    }

}

void CScouting_NorMon_1::Exit(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
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
