#include "pch.h"
#include "StateObject.h"
CStateObject::CStateObject() : m_pGameInstance (CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

void CStateObject::Free()
{
	Safe_Release(m_pGameInstance);
}
