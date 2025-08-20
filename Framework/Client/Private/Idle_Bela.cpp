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
    pContainer->Switch_AnimState(ENUM_CLASS(BOSS_SISTER::NORMAL));
    pContainer->Switch_Anim("Idle_Freezes", true);
}

void CIdle_Bela::Update(CBela* pContainer, _float fDeltatime)
{
    if (pContainer->Get_State().isDamage)
    {
        pContainer->Switch_State(TEXT("Damage"));
    }
    else if (pContainer->Get_State().isChase)
    {
        pContainer->Switch_State(TEXT("Chase"));
    }
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
