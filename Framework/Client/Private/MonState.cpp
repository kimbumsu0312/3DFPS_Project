#include "pch.h"
#include "MonState.h"
CMonState::CMonState() : CStateObject()
{
}

HRESULT CMonState::Initalize(void* pArg)
{
	Mon_STATE_DESC* pDesc = static_cast<Mon_STATE_DESC*>(pArg);

	m_pAnimState = pDesc->pAnimState;
	m_pAnimTag = pDesc->pAnimTag;
	m_pStateTag = pDesc->pStateTag;
	m_pIsAnimFinsh = pDesc->pIsAnimFinsh;
	m_pIsAnimLoop = pDesc->pIsAnimLoop;
	return S_OK;
}

void CMonState::Free()
{
	Safe_Release(m_pGameInstance);
}
