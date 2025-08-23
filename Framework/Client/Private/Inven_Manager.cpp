#include "pch.h"
#include "Inven_Manager.h"
#include "Client_Item.h"

IMPLEMENT_SINGLETON(CInventory_Manager)

CInventory_Manager::CInventory_Manager()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

HRESULT CInventory_Manager::Initialize(const INVENTORY_DESC& pDesc)
{
	m_vInvenCenter = pDesc.vInvenCenter;
	m_iSlotNumX = pDesc.iSlotNumX;
	m_iSlotNumY = pDesc.iSlotNumY;
	m_iSlotSize = pDesc.iSlotSize;

	m_InvenWinSize.left = 0;
	m_InvenWinSize.top = 0;
	m_InvenWinSize.right = m_iSlotNumX * m_iSlotSize;
	m_InvenWinSize.bottom = m_iSlotNumY * m_iSlotSize;

	m_InvenWinPos.left = m_vInvenCenter.x - ((m_InvenWinSize.right - m_InvenWinSize.left) * 0.5);
	m_InvenWinPos.top = m_vInvenCenter.y - ((m_InvenWinSize.bottom - m_InvenWinSize.top) * 0.5);
	m_InvenWinPos.right = m_vInvenCenter.x + ((m_InvenWinSize.right - m_InvenWinSize.left) * 0.5);
	m_InvenWinPos.bottom = m_vInvenCenter.y + ((m_InvenWinSize.bottom - m_InvenWinSize.top) * 0.5);

	m_InvenSlot.resize(m_iSlotNumY);

	for (_uint i = 0; i < m_iSlotNumY; ++i)
	{
		m_InvenSlot[i].resize(m_iSlotNumX);
	}

	return S_OK;
}

HRESULT CInventory_Manager::Level_Init(_uint iLayerLevel, _wstring szLayerTag)
{
	if (iLayerLevel <= 0)
		return E_FAIL;

	m_iLayerLevel = iLayerLevel;
	m_szLayertag = szLayerTag;
	return S_OK;
}

_bool CInventory_Manager::Add_ItemSlot(_int iItemIndex, _wstring szPoolPath)
{
	_int ItemSizeX = g_ItemData[iItemIndex].m_iInvenSizeX;
	_int ItemSizeY = g_ItemData[iItemIndex].m_iInvenSizeY;

	_int iGridX = { -1 };
	_int iGridY = { -1 };
	if (AddItem_Check(ItemSizeX, ItemSizeY, iGridX, iGridY))
	{
		for (_int i = iGridY; i < iGridY + ItemSizeY; ++i)
		{
			for (_int j = iGridX; j < iGridX + ItemSizeX; ++j)
			{
				m_InvenSlot[i][j] = true;
			}
		}
		CInvenItem::ITEM_DESC Desc = Setting_Item(iItemIndex, iGridX, iGridY);
		m_pGameInstance->Add_Pool_ToLayer(szPoolPath, m_iLayerLevel, m_szLayertag, &Desc);
		return true;
	}

	return false;
}

void CInventory_Manager::Add_Item(CInvenItem* pItem)
{
	if (pItem == nullptr)
		return;

	m_InvenItems.push_back(pItem);

}

_bool CInventory_Manager::Erase_ItemSlot(CInvenItem* pItem)
{
	for (auto iter = m_InvenItems.begin(); iter < m_InvenItems.end(); iter++)
	{
		if (*iter == pItem)
		{
			INVEN_ITEM itemData = (*iter)->Get_ItemData();
			(*iter)->SetDead();

			_int ItemSizeX = itemData.iInvenSizeX;
			_int ItemSizeY = itemData.iInvenSizeY;
			_int iGridX = itemData.iItemGridX;
			_int iGridY = itemData.iItemGridY;

			for (_int i = iGridY; i < iGridY + ItemSizeY; ++i)
			{
				for (_int j = iGridX; j < iGridX + ItemSizeX; ++j)
				{
					m_InvenSlot[i][j] = false;
				}
			}

			Safe_Release(*iter);
			m_InvenItems.erase(iter);
			return true;
		}
	}

	return false;
}

_bool CInventory_Manager::AddItem_Check(const _int& pSizeX, const _int& pSizeY, _int& iItemGridX, _int& iItemGridY)
{
	for (_uint i = 0; i < m_iSlotNumY; ++i )
	{
		for (_uint j = 0; j < m_iSlotNumX; ++j)
		{
			if(m_InvenSlot[i][j])
				continue;

			if (j + pSizeX > m_iSlotNumX)
				break;


			if (ItemSlot_Check(pSizeX, pSizeY, j, i))
			{
				iItemGridX = j;
				iItemGridY = i;
				return true;
			}
		}

		if (i + pSizeY > m_iSlotNumY)
			break;
	}
	return false;
}

_bool CInventory_Manager::ItemSlot_Check(const _int& pSizeX, const _int& pSizeY, _int iStartX, _int iStartY)
{
	for (_int i = iStartY; i < iStartY + pSizeY; ++i)
	{
		for (_int j = iStartX; j < iStartX + pSizeX; ++j)
		{
			if (m_InvenSlot[i][j])
				return false;
		}
	}
	return true;
}

CInvenItem::ITEM_DESC CInventory_Manager::Setting_Item(_int iItemIndex, _int iItemGridX, _int iItemGridY)
{
	CInvenItem::ITEM_DESC Desc{};

	Desc.ItemData.iItemIndex = g_ItemData[iItemIndex].m_iItemID;
	Desc.ItemData.iInvenSizeX = g_ItemData[iItemIndex].m_iInvenSizeX;
	Desc.ItemData.iInvenSizeY = g_ItemData[iItemIndex].m_iInvenSizeY;
	Desc.vSize = g_ItemData[iItemIndex].m_vSize;
	Desc.vMinUV = g_ItemData[iItemIndex].m_vMinUV;
	Desc.vMaxUV = g_ItemData[iItemIndex].m_vMaxUV;
	Desc.ItemData.iItemGridX = iItemGridX;
	Desc.ItemData.iItemGridY = iItemGridY;

	Convert_InvenWorld(iItemGridX, iItemGridY);

	Desc.vPos.x = iItemGridX;
	Desc.vPos.y = iItemGridY;

	return Desc;
}


void CInventory_Manager::Convert_InvenGrid(_int& iX, _int& iY)
{
	iX = (iX - m_InvenWinPos.left) / m_iSlotSize;
	iY = (iY - m_InvenWinPos.top) / m_iSlotSize;
}

void CInventory_Manager::Convert_InvenWorld(_int& iX, _int& iY)
{
	iX = m_InvenWinPos.left + iX * m_iSlotSize;
	iY = m_InvenWinPos.top + iY * m_iSlotSize;
}

void CInventory_Manager::Free()
{
	__super::Free();
	for (auto Item : m_InvenItems)
	{
		Safe_Release(Item);
	}
	Safe_Release(m_pGameInstance);
}
