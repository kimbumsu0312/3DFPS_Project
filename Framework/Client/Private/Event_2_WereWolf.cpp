#include "pch.h"
#include "Event_2_WereWolf.h"
#include "Monster_WereWolf.h"

CEvent_2_WereWolf::CEvent_2_WereWolf()
{
}

HRESULT CEvent_2_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CEvent_2_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = (ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::ATTACK));
    pContainer->Switch_Anim("Attack_Door", false);
    m_fMoveTime = 0.f;
    m_eEvent = Event::END;
    m_iCount = 0;
    pContainer->Get_Transform()->Set_State(STATE::POSITION, _vector{ -47.46f, -3.63f, 30.14f, 1.f });

}

void CEvent_2_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
        pContainer->Get_Transform()->Set_State(STATE::POSITION, _vector{ -47.46f, -3.63f, 30.14f, 1.f });
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        
        if (m_eEvent == Event::END && *pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Switch_Anim("Attack_L", false);
            m_eEvent = Event::TURN;
        }
        else if (m_eEvent == Event::TURN && *pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Switch_Anim("Attack_Rush_Loop", true);
            m_eAnimState = STATE_ANIM::END;
        }

    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        m_fMoveTime += fDeltatime;
        if (m_fMoveTime > 5.f)
        {
            pContainer->Get_BlackBoard()->Set_Data().IsChase = false;
            pContainer->Get_BlackBoard()->Set_Data().IsEvent_2 = false;
        }
    }
}

void CEvent_2_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
    pContainer->Event3_Create();
}

CEvent_2_WereWolf* CEvent_2_WereWolf::Create(void* pArg)
{
    CEvent_2_WereWolf* pInstance = new CEvent_2_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CEvent_2_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEvent_2_WereWolf::Free()
{
    __super::Free();
}
