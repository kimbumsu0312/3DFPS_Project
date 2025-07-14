#include "pch.h"
#include "Player_Manager.h"

IMPLEMENT_SINGLETON(CPlayer_Manager)

CPlayer_Manager::CPlayer_Manager()
{
}

HRESULT CPlayer_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);

	m_fMaxHp = 100;
	m_fCulHp = m_fMaxHp;
	m_fPreHp = 0.f;
	return S_OK;
}

void CPlayer_Manager::Player_Hp(_int iValue)
{
	m_fCulHp += iValue;

	if (m_fPreHp != m_fCulHp)
	{
		if (m_fCulHp >= m_fMaxHp * 0.75)
			m_pGameInstance->Publish(Event_Player_Hp_Set{ {0.f, 0.6f, 0.f, 0.7f} });
		else if (m_fCulHp >= m_fMaxHp * 0.50)
			m_pGameInstance->Publish(Event_Player_Hp_Set{ {0.6f, 0.6f, 0.f, 0.7f} });
		else if (m_fCulHp >= m_fMaxHp * 0.25)
			m_pGameInstance->Publish(Event_Player_Hp_Set{ {0.7f, 0.3f, 0.f, 0.7f} });
		else if (m_fCulHp < m_fMaxHp * 0.25)
			m_pGameInstance->Publish(Event_Player_Hp_Set{ {0.7f, 0.f, 0.f, 0.7f} });
	}
}

void CPlayer_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
