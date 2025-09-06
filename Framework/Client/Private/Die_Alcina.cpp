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
    m_eAnimState = STATE_ANIM::LOOP;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::NORMAL);
    pContainer->Switch_Anim("Idle", true);
}

void CDie_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
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
