#include "pch.h"
#include "Event_1_WereWolf.h"
#include "Monster_WereWolf.h"

CEvent_1_WereWolf::CEvent_1_WereWolf()
{
}

HRESULT CEvent_1_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CEvent_1_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = (ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::ATTACK));
    pContainer->Switch_Anim("Attack_Rush_Howling", false);
    m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf1"), true);
}

void CEvent_1_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Switch_Anim("Attack_Rush_Loop", true);
            m_eAnimState = STATE_ANIM::END;
        }
        //_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
        //_vector vMonPos = pContainer->Get_Transform()->Get_State(STATE::POSITION);
        //_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

        //if (fDis <= 3.f)
        //{
        //    pContainer->Switch_Anim("Attack_Rush_End", true);
        //    m_eAnimState = STATE_ANIM::END;
        //}
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        m_fLightTime++;

        if(m_fLightTime > 1.f)
            m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf2"), true);
        if(m_fLightTime > 3.f)
            m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf3"), true);
        if(m_fLightTime > 5.f)
            m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf4"), true);
        //pContainer->Target_LookTurn(fDeltatime);
        //if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        //{
        //    pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
        //    pContainer->Get_BlackBoard()->Set_Data().IsEvent_1 = false;
        //}
    }


}

void CEvent_1_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CEvent_1_WereWolf* CEvent_1_WereWolf::Create(void* pArg)
{
    CEvent_1_WereWolf* pInstance = new CEvent_1_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CEvent_1_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEvent_1_WereWolf::Free()
{
    __super::Free();
}
