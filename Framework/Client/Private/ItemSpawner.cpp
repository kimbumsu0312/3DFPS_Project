#include "pch.h"
#include "ItemSpawner.h"
#include "Client_Item.h"
#include "PoolWorldItem.h"

IMPLEMENT_SINGLETON(CItemSpawner)

CItemSpawner::CItemSpawner()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

HRESULT CItemSpawner::Level_Init(_uint iLayerLevel, _wstring szLayerTag)
{
	if (iLayerLevel <= 0)
		return E_FAIL;

	m_iLayerLevel = iLayerLevel;
	m_szLayertag = szLayerTag;
	return S_OK;
}

HRESULT CItemSpawner::Spawn_Item(_uint iItemIndex, _vector vPos, _int iNaviIndex)
{
	_wstring szPoolPath = g_3DItemData[iItemIndex].m_szPoolPath;
	CPoolWorld_Item::POOLITEM_DESC Desc{};
	Desc.vPos = vPos;
	Desc.vNaviIndex = iNaviIndex;
	Desc.szPoolPath = TEXT("Pool_Item");
	m_pGameInstance->Add_Pool_ToLayer(szPoolPath, m_iLayerLevel, m_szLayertag, &Desc);
	return S_OK;
}

void CItemSpawner::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
