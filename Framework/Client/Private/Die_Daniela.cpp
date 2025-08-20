#include "pch.h"
#include "Die_Daniela.h"
#include "Daniela.h"

CDie_Daniela::CDie_Daniela()
{
}

HRESULT CDie_Daniela::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_Daniela::Enter(CDaniela* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_AnimState(ENUM_CLASS(BOSS_SISTER::DAMAGE));
    pContainer->Switch_Anim("Freezes_Die", false);
}

void CDie_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
}

void CDie_Daniela::Exit(CDaniela* pContainer)
{
}

CDie_Daniela* CDie_Daniela::Create(void* pArg)
{
    CDie_Daniela* pInstance = new CDie_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_Daniela::Free()
{
    __super::Free();
}
