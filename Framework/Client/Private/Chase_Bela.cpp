#include "pch.h"
#include "Chase_Bela.h"
#include "Bela.h"

CChase_Bela::CChase_Bela()
{
}

HRESULT CChase_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CChase_Bela::Enter(CBela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    //pContainer->Switch_AnimState(ENUM_CLASS(CBela::BOSS_SISTER::NORMAL));
    pContainer->Switch_Anim("Run_F", true);

}

void CChase_Bela::Update(CBela* pContainer, _float fTimeDelta)
{
    pContainer->Target_LookTurn(fTimeDelta);

}

void CChase_Bela::Exit(CBela* pContainer)
{
}

CChase_Bela* CChase_Bela::Create(void* pArg)
{
    CChase_Bela* pInstance = new CChase_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CChase_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChase_Bela::Free()
{
    __super::Free();
}
