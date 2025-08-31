#include "pch.h"
#include "Idle_Alcina.h"

CIdle_Alcina::CIdle_Alcina()
{
}

HRESULT CIdle_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CIdle_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_AnimState(ENUM_CLASS(BOSS_SISTER::NORMAL));
    pContainer->Switch_Anim("Idle", true);
}

void CIdle_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
}

void CIdle_Alcina::Exit(CAlcina* pContainer)
{
}

CIdle_Alcina* CIdle_Alcina::Create(void* pArg)
{
    CIdle_Alcina* pInstance = new CIdle_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_Alcina::Free()
{
    __super::Free();
}
