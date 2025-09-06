#include "pch.h"
#include "Idle_Daniela.h"
#include "Daniela.h"

CIdle_Daniela::CIdle_Daniela()
{
}

HRESULT CIdle_Daniela::Initalize(void* pArg)
{
	return S_OK;
}

void CIdle_Daniela::Enter(CDaniela* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_AnimState(ENUM_CLASS(CDaniela::ANIM_STATE::NORMAL));
    pContainer->Switch_Anim("Idle", true);
}

void CIdle_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
}

void CIdle_Daniela::Exit(CDaniela* pContainer)
{
}

CIdle_Daniela* CIdle_Daniela::Create(void* pArg)
{
    CIdle_Daniela* pInstance = new CIdle_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_Daniela::Free()
{
	__super::Free();
}
