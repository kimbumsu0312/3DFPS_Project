#include "pch.h"
#include "StateObject.h"
CStateObject::CStateObject() : m_pGameInstance (CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CStateObject::Initalize(void* pArg)
{
	return S_OK;
}

void CStateObject::Free()
{
	Safe_Release(m_pGameInstance);
}
