#include "pch.h"
#include "Idle_Werewolf.h"
#include "Monster_WereWolf.h"

CIdle_WereWolf::CIdle_WereWolf()
{
}

HRESULT CIdle_WereWolf::Initalize(void* pArg)
{
    return S_OK;
}

void CIdle_WereWolf::Enter(CMonster_WereWolf* pContainer)
{

}

void CIdle_WereWolf::Update(CMonster_WereWolf* pContainer, _float fDeltatime)
{

}

void CIdle_WereWolf::Exit(CMonster_WereWolf* pContainer)
{
}

CIdle_WereWolf* CIdle_WereWolf::Create(void* pArg)
{
    CIdle_WereWolf* pInstance = new CIdle_WereWolf();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_WereWolf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_WereWolf::Free()
{
    __super::Free();
}
