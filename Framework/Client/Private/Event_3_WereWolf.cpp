#include "pch.h"
#include "Event_3_WereWolf.h"
#include "Monster_WereWolf.h"

CEvent_3_WereWolf::CEvent_3_WereWolf()
{
}

HRESULT CEvent_3_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CEvent_3_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = (ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::ATTACK));
    pContainer->Switch_Anim("Attack_Jump", false);
}

void CEvent_3_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
        m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_2));
        m_pGameInstance->PlaySoundW(TEXT("Wolf_Howl.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_2), g_fBGMVolume);
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
            pContainer->Get_BlackBoard()->Set_Data().IsEvent_3 = false;
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
    }
}

void CEvent_3_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CEvent_3_WereWolf* CEvent_3_WereWolf::Create(void* pArg)
{
    CEvent_3_WereWolf* pInstance = new CEvent_3_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CEvent_3_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEvent_3_WereWolf::Free()
{
    __super::Free();
}
