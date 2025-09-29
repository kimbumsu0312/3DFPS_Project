#include "pch.h"
#include "Die_Werewolf.h"
#include "Monster_WereWolf.h"

CDie_WereWolf::CDie_WereWolf()
{
}

HRESULT CDie_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_WereWolf::Enter(CMonster_WereWolf* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = (ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::DAMAGE));
    pContainer->Switch_Anim("Die", false);
    m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_2));
    m_pGameInstance->PlaySoundW(TEXT("Wolf_StandUp.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_2), g_fBGMVolume);
}

void CDie_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (m_isNoies)
        {
            pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime * 0.5f;
            if (pContainer->Get_BlackBoard()->Get_Data().fNoies > 1.f)
            {
                pContainer->SetDead();
                CItemSpawner::GetInstance()->Spawn_Item(7, pContainer->Get_Transform()->Get_State(STATE::POSITION), pContainer->Get_Navi()->Get_CulIndex());
                m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf1"), false);
                m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf2"), false);
                m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf3"), false);
                m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf4"), false);

            }
        }
        else if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_isNoies = true;
        }

    }
    else if (m_eAnimState == STATE_ANIM::END)
    {

    }
}

void CDie_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CDie_WereWolf* CDie_WereWolf::Create(void* pArg)
{
    CDie_WereWolf* pInstance = new CDie_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_WereWolf::Free()
{
    __super::Free();
}
