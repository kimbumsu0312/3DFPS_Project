#include "pch.h"
#include "Walk_Werewolf.h"
#include "Monster_WereWolf.h"

CWalk_WereWolf::CWalk_WereWolf()
{
}

HRESULT CWalk_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CWalk_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    if (m_bIsStart)
    {
        m_eAnimState = STATE_ANIM::START;
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = (ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::NORMAL));
        pContainer->Switch_Anim("Walk_F_Start", false);
    }
    else
    {
        m_eAnimState = STATE_ANIM::LOOP;
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = (ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::NORMAL));
        pContainer->Switch_Anim("Walk_F_Loop", true);
    }
}

void CWalk_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (m_bIsStart)
        {
            if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            {
                pContainer->Switch_Anim("Walk_F_Loop", true);
                m_bIsStart = false;
            }
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {

    }
}

void CWalk_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CWalk_WereWolf* CWalk_WereWolf::Create(void* pArg)
{
    CWalk_WereWolf* pInstance = new CWalk_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CWalk_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWalk_WereWolf::Free()
{
    __super::Free();
}
