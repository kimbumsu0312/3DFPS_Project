#include "pch.h"
#include "Die_Bela.h"
#include "Bela.h"

CDie_Bela::CDie_Bela()
{
}

HRESULT CDie_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_Bela::Enter(CBela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CBela::ANIM_STATE::DAMAGE);
    pContainer->Switch_Anim("Freezes_Die", false);

}

void CDie_Bela::Update(CBela* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        _float fNoies = pContainer->Get_BlackBoard()->Set_Data().fNoies -= fDeltatime * 0.5f;
        
        
        if (pContainer->Get_BlackBoard()->Get_Data().fNoies <= 0.f)
        {
            m_eAnimState = STATE_ANIM::LOOP;
            pContainer->Get_BlackBoard()->Set_Data().bIsFly = false;
            pContainer->Spawn_EffectReset();
            pContainer->Get_BlackBoard()->Set_Data().bIsSpawnFly = true;

            pContainer->Get_BlackBoard()->Set_Data().fNoies = 0.f;
            pContainer->Get_BlackBoard()->Set_Data().IsFreezes = true;
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime * 0.1f;

        if (pContainer->Get_BlackBoard()->Get_Data().IsDamage == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().bIsSpawnFly = false;
            m_eAnimState = STATE_ANIM::END;
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        pContainer->Get_BlackBoard()->Set_Data().fFreezes += fDeltatime * 0.5f;

        if (pContainer->Get_BlackBoard()->Get_Data().fFreezes >= 1.f)
        {
            pContainer->SetDead();
            m_pGameInstance->Publish(Event_OpenDoor{ true , 1.f });
        }
    }
}

void CDie_Bela::Exit(CBela* pContainer)
{
}

CDie_Bela* CDie_Bela::Create(void* pArg)
{
    CDie_Bela* pInstance = new CDie_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_Bela::Free()
{
    __super::Free();
}
