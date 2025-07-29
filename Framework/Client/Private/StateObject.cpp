#include "pch.h"
#include "StateObject.h"
CStateObject::CStateObject() : m_pGameInstance (CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CStateObject::Initalize(void* pArg)
{
	STATE_DESC* pDesc = static_cast<STATE_DESC*>(pArg);

	m_pState = pDesc->pState;
	m_pWeaponState = pDesc->pWeaponState;
	m_pAnimTag = pDesc->pAnimTag;
	m_pStateTag = pDesc->pStateTag;
	m_pIsAnimFinsh = pDesc->pIsAnimFinsh;
	m_pIsAnimLoop = pDesc->pIsAnimLoop;
	return S_OK;
}

void CStateObject::Free()
{
	Safe_Release(m_pGameInstance);
}
