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
	m_iDamage = 20;
	m_fMaxHp = 100;
	m_fCulHp = m_fMaxHp;
	m_fPreHp = 0.f;
	m_iCoin = 1000;

	for (_int i = 0; i < ENUM_CLASS(QUICKSLOT::END); ++i)
	{
		m_iQuickSlot[i] = -1;
	}
		

	return S_OK;
}

void CPlayer_Manager::Player_Hp(_int iValue)
{
	if (m_bIsDamage)
		return;

	m_bIsDamage = true;
	m_fCulHp += iValue;
	m_pGameInstance->Publish(Event_Player_Hp_UI_Open{ 0 });
	m_pGameInstance->Publish(Event_OnDamageUI_OPEN{});
	
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

void CPlayer_Manager::Add_QuickSlotItem(_int iSlotIndex, _int iItemIndex)
{
	m_iQuickSlot[iSlotIndex - 1] = iItemIndex;
}

void CPlayer_Manager::Selete_Slot(_int i)
{
	m_iSeleteItem = - 1;
	_int iSlotIndex = i - 1;

	if (m_iQuickSlot[iSlotIndex] != -1)
	{
		m_iSeleteItem = m_iQuickSlot[iSlotIndex];
	}
	else
		m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::END });

}

_bool CPlayer_Manager::Gun_Shoting()
{
	if (CInven_Manager::GetInstance()->Bullet_Check(m_iSeleteItem) == true)
		return true;
	
	m_pGameInstance->Publish(Event_Announce_UI_OPEN{ 1, 0, TEXT("장전된 총알이 부족합니다."), RENDERGROUP::PRIORITY_UI });
	
	_int ItemIndex = Get_SeleteItemIndex();
	_int iGunBullet = {};
	_int iInvenBullet = {};

	if (CInven_Manager::GetInstance()->Get_BulletCount(ItemIndex, iGunBullet, iInvenBullet))
		m_pGameInstance->Publish(Event_BulletCount_UI_OPEN{ ItemIndex, iGunBullet, iInvenBullet });
	return false;
}

_bool CPlayer_Manager::Reload()
{
	if (CInven_Manager::GetInstance()->Reload_Check(m_iSeleteItem) == true)
		return true;
	
	m_pGameInstance->Publish(Event_Announce_UI_OPEN{ 1, 0, TEXT("인벤토리 내 총알이 부족합니다."), RENDERGROUP::PRIORITY_UI });

	_int ItemIndex = Get_SeleteItemIndex();
	_int iGunBullet = {};
	_int iInvenBullet = {};

	if (CInven_Manager::GetInstance()->Get_BulletCount(ItemIndex, iGunBullet, iInvenBullet))
		m_pGameInstance->Publish(Event_BulletCount_UI_OPEN{ ItemIndex, iGunBullet, iInvenBullet });
	return false;
}

void CPlayer_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
