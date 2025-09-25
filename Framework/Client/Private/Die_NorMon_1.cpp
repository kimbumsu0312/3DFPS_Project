#include "pch.h"
#include "Die_NorMon_1.h"
#include "Monster_Normal.h"
#include "ItemSpawner.h"
CDie_Normon_1::CDie_Normon_1()
{
}

HRESULT CDie_Normon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_Normon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::DAMAGE);
    pContainer->Switch_Anim("Die", false);

    m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_1));
    m_pGameInstance->PlaySoundW(TEXT("Normon_1_Dead.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_1), g_fBGMVolume);
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;
}

void CDie_Normon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
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
            }
        }
        else if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_isNoies = true;
            CItemSpawner::GetInstance()->Spawn_Item(pContainer->Get_BlackBoard()->Get_Data().iDropItemIndex, pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_CulNaviIndex());
        }
    }
}

void CDie_Normon_1::Exit(CMonster_Normal* pContainer)
{
}

CDie_Normon_1* CDie_Normon_1::Create(void* pArg)
{
    CDie_Normon_1* pInstance = new CDie_Normon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_Normon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_Normon_1::Free()
{
    __super::Free();
}
