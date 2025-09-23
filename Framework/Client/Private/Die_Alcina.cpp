#include "pch.h"
#include "Die_Alcina.h"

CDie_Alcina::CDie_Alcina()
{
}

HRESULT CDie_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::ATTACK);
    pContainer->Switch_Anim("Die", true);
}

void CDie_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    pContainer->Target_LookTurn(fDeltatime * 3.f);
    if (m_eAnimState == STATE_ANIM::START)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies -= fDeltatime * 1.3f;
        if (pContainer->Get_BlackBoard()->Get_Data().fNoies <= 0.f)
        {
            m_eAnimState = STATE_ANIM::LOOP;
            pContainer->Get_BlackBoard()->Set_Data().bIsFly = false;
            pContainer->Spawn_EffectReset();
            pContainer->Get_BlackBoard()->Set_Data().bIsSpawnFly = true;

            pContainer->Get_BlackBoard()->Set_Data().fNoies = 0.f;
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime * 0.5f;
        if (pContainer->Get_BlackBoard()->Get_Data().fNoies > 1.f)
            pContainer->SetDead();
    }
}

void CDie_Alcina::Exit(CAlcina* pContainer)
{
}

CDie_Alcina* CDie_Alcina::Create(void* pArg)
{
    CDie_Alcina* pInstance = new CDie_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_Alcina::Free()
{
    __super::Free();
}
