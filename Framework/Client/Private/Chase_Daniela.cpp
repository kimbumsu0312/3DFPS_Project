#include "pch.h"
#include "Chase_Daniela.h"
#include "Daniela.h"

CChase_Daniela::CChase_Daniela()
{
}

HRESULT CChase_Daniela::Initalize(void* pArg)
{
    return S_OK;
}

void CChase_Daniela::Enter(CDaniela* pContainer)
{
    if(!m_bIsLight)
       m_pGameInstance->OnOff_Light(TEXT("Light_Daniela"), true);
    m_eAnimState = STATE_ANIM::START;
    pContainer->Switch_AnimState(ENUM_CLASS(CDaniela::ANIM_STATE::NORMAL));
    pContainer->Switch_Anim("Walk_Loop", true);

    pContainer->SetUp_Node();
}

void CChase_Daniela::Update(CDaniela* pContainer, _float fTimeDelta)
{
    pContainer->Target_LookTurn_Navi(fTimeDelta);
}

void CChase_Daniela::Exit(CDaniela* pContainer)
{
}

CChase_Daniela* CChase_Daniela::Create(void* pArg)
{
    CChase_Daniela* pInstance = new CChase_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CChase_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChase_Daniela::Free()
{
    __super::Free();
}
