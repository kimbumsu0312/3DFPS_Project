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
    m_eAnimState = STATE_ANIM::LOOP;
    //pContainer->Switch_AnimState(ENUM_CLASS(CBela::BOSS_SISTER::DAMAGE));
    pContainer->Switch_Anim("Freezes_Die", false);
}

void CDie_Bela::Update(CBela* pContainer, _float fDeltatime)
{
    pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime * 0.5f;
    if (pContainer->Get_BlackBoard()->Get_Data().fNoies > 1.f)
        pContainer->SetDead();
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
