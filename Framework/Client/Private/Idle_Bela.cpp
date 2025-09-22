#include "pch.h"
#include "Idle_Bela.h"
#include "Bela.h"

CIdle_Bela::CIdle_Bela()
{
}

HRESULT CIdle_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CIdle_Bela::Enter(CBela* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CBela::ANIM_STATE::NORMAL);
    pContainer->Switch_Anim("Idle_Freezes", true);
}

void CIdle_Bela::Update(CBela* pContainer, _float fDeltatime)
{

}

void CIdle_Bela::Exit(CBela* pContainer)
{
}

CIdle_Bela* CIdle_Bela::Create(void* pArg)
{
    CIdle_Bela* pInstance = new CIdle_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_Bela::Free()
{
    __super::Free();
}
