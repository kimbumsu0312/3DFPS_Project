#include "pch.h"
#include "PlayerState.h"
CPlayerState::CPlayerState() : CStateObject()
{
}

HRESULT CPlayerState::Initalize(void* pArg)
{
	Player_STATE_DESC* pDesc = static_cast<Player_STATE_DESC*>(pArg);

	m_pWeaponState = pDesc->pWeaponState;
	m_pAnimTag = pDesc->pAnimTag;
	m_pStateTag = pDesc->pStateTag;
	m_pIsAnimFinsh = pDesc->pIsAnimFinsh;
	m_pIsAnimLoop = pDesc->pIsAnimLoop;
	m_pNextWeaponState = pDesc->pNextWeaponState;
	return S_OK;
}

void CPlayerState::Free()
{
	Safe_Release(m_pGameInstance);
}
