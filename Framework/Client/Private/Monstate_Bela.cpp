#include "pch.h"
#include "MonState_Bela.h"
CMonState_Bela::CMonState_Bela() : CStateObject(), m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

void CMonState_Bela::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
