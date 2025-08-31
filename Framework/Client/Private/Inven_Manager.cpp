#include "pch.h"
#include "Inven_Manager.h"
#include "Client_Item.h"

IMPLEMENT_SINGLETON(CInven_Manager)

CInven_Manager::CInven_Manager()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

HRESULT CInven_Manager::Initialize(const INVENTORY_DESC& pDesc)
{
	m_vInvenCenter = pDesc.vInvenCenter;
	m_iSlotNumX = pDesc.iSlotNumX;
	m_iSlotNumY = pDesc.iSlotNumY;
	m_iSlotSize = pDesc.iSlotSize;

	m_InvenWinSize.left = 0;
	m_InvenWinSize.top = 0;
	m_InvenWinSize.right = m_iSlotNumX * m_iSlotSize;
	m_InvenWinSize.bottom = m_iSlotNumY * m_iSlotSize;

	m_InvenWinPos.left = _int(m_vInvenCenter.x - ((m_InvenWinSize.right - m_InvenWinSize.left) * 0.5f));
	m_InvenWinPos.top = _int(m_vInvenCenter.y - ((m_InvenWinSize.bottom - m_InvenWinSize.top) * 0.5f));
	m_InvenWinPos.right = _int(m_vInvenCenter.x + ((m_InvenWinSize.right - m_InvenWinSize.left) * 0.5f));
	m_InvenWinPos.bottom = _int(m_vInvenCenter.y + ((m_InvenWinSize.bottom - m_InvenWinSize.top) * 0.5f));

	m_InvenSlot.resize(m_iSlotNumY);

	for (_uint i = 0; i < m_iSlotNumY; ++i)
	{
		m_InvenSlot[i].resize(m_iSlotNumX);
	}

	return S_OK;
}

HRESULT CInven_Manager::Level_Init(_uint iLayerLevel, _wstring szLayerTag)
{
	if (iLayerLevel <= 0)
		return E_FAIL;

	m_iLayerLevel = iLayerLevel;
	m_szLayertag = szLayerTag;
	return S_OK;
}

_bool CInven_Manager::Add_ItemSlot(_int iItemIndex, _wstring szPoolPath)
{
	if (g_ItemData[iItemIndex].m_eType == ITEM_TYPE::ITEM)
	{
		for (auto Item : m_InvenItems)
		{

			if (Item->Get_ItemData().iItemIndex == iItemIndex)
			{
				if (Item->Get_ItemData().iItemCount + g_ItemData[iItemIndex].m_iItemValue > g_ItemData[iItemIndex].m_iMaxItem)
					continue;

				Item->Add_ItemValue(g_ItemData[iItemIndex].m_iItemValue);
				m_pGameInstance->Publish(Event_Announce_UI_OPEN{ 0, (_uint)iItemIndex, TEXT(""), RENDERGROUP::PRIORITY_UI });
				return true;
			}

		}
	}
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
		m_pGameInstance->Publish(Event_Announce_UI_OPEN{ 0, (_uint)iItemIndex, TEXT(""), RENDERGROUP::PRIORITY_UI });
		return true;
	}
	m_pGameInstance->Publish(Event_Announce_UI_OPEN{ 1, (_uint)iItemIndex, TEXT("인벤토리 공간이 부족합니다."), RENDERGROUP::PRIORITY_UI });
	return false;
}

_bool CInven_Manager::Change_ItemSlot(CInvenItem* pItem)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	
	_int iMouseX = ptMouse.x;
	_int iMouseY = ptMouse.y;

	Convert_InvenGrid(iMouseX, iMouseY);

	INVEN_ITEM Desc = pItem->Get_ItemData();
	
	_int ItemSizeX = {};
	_int ItemSizeY = {};
	if (pItem->Get_Rtation())
	{
		ItemSizeX = g_ItemData[Desc.iItemIndex].m_iInvenSizeY;
		ItemSizeY = g_ItemData[Desc.iItemIndex].m_iInvenSizeX;
	}
	else
	{
		ItemSizeX = g_ItemData[Desc.iItemIndex].m_iInvenSizeX;
		ItemSizeY = g_ItemData[Desc.iItemIndex].m_iInvenSizeY;
	}

	_int iStartGridX = iMouseX - ((ItemSizeX) / 2);
	_int iStartGridY = iMouseY - ((ItemSizeY) / 2);

	if (iStartGridX < 0 || iStartGridY < 0 || iStartGridX + ItemSizeX >(_int)m_iSlotNumX || iStartGridY + ItemSizeY >(_int)m_iSlotNumY)
		return false;
	
	if (ItemSlot_Check(iStartGridX, iStartGridY, ItemSizeX, ItemSizeY, Desc))
	{
		for (_int i = Desc.iItemGridY; i < Desc.iItemGridY + Desc.iInvenSizeY; ++i)
		{
			for (_int j = Desc.iItemGridX; j < Desc.iItemGridX + Desc.iInvenSizeX; ++j)
			{
				m_InvenSlot[i][j] = false;
			}
		}

		for (_int i = iStartGridY; i < iStartGridY + ItemSizeY; ++i)
		{
			for (_int j = iStartGridX; j < iStartGridX + ItemSizeX; ++j)
			{
				m_InvenSlot[i][j] = true;
			}
		}

		_float2 vCenter{};


		vCenter.x = ((_float)iStartGridX + _float(iStartGridX + ItemSizeX - 1)) * 0.5f;
		vCenter.y = ((_float)iStartGridY + _float(iStartGridY + ItemSizeY - 1)) * 0.5f;

		Convert_InvenWorld(vCenter.x, vCenter.y);

		pItem->Change_Slot(vCenter, iStartGridX, iStartGridY);
		return true;
	}

	return false;

}

void CInven_Manager::Add_Item(CInvenItem* pItem)
{
	if (pItem == nullptr)
		return;

	m_InvenItems.push_back(pItem);
	Safe_AddRef(pItem);

}

_bool CInven_Manager::Erase_ItemSlot(CInvenItem* pItem)
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

void CInven_Manager::InvenOpen(_bool IsOpen)
{
	m_IsInvenOpen = IsOpen;
	for (auto Item : m_InvenItems)
	{
		Item->Set_Render(IsOpen);
	}
}

const CInven_Manager::INVENTORY_DESC& CInven_Manager::Get_InvenData()
{
	INVENTORY_DESC Data{};
	Data.iSlotNumX = m_iSlotNumX;
	Data.iSlotNumY = m_iSlotNumY;
	Data.iSlotSize = m_iSlotSize;
	Data.vInvenCenter = m_vInvenCenter;

	return Data;

}

_bool CInven_Manager::AddItem_Check(const _int& pSizeX, const _int& pSizeY, _int& iItemGridX, _int& iItemGridY)
{
	for (_uint i = 0; i < m_iSlotNumY; ++i )
	{
		if (i + pSizeY > m_iSlotNumY)
			break;

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


	}
	return false;
}

_bool CInven_Manager::ItemSlot_Check(const _int& pSizeX, const _int& pSizeY, _int iStartX, _int iStartY)
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

_bool CInven_Manager::ItemSlot_Check(_int iStartX, _int iStartY, _int iSizeX, _int iSizeY, const INVEN_ITEM& Desc)
{

	if (iStartX < 0 || iStartY < 0 || iStartX + iSizeX > (_int)m_iSlotNumX || iStartY + iSizeY >(_int)m_iSlotNumY)
		return false;

	_int iItemLeft = Desc.iItemGridX;
	_int iItemTop = Desc.iItemGridY;
	_int iItemRight = Desc.iItemGridX + Desc.iInvenSizeX - 1;
	_int iItemBottom = Desc.iItemGridY + Desc.iInvenSizeY - 1;

	for (_int i = iStartY; i < iStartY + iSizeY; ++i)
	{
		for (_int j = iStartX; j < iStartX + iSizeX; ++j)
		{
			if (iItemTop <= i && iItemBottom >= i && iItemLeft <= j && iItemRight >= j)
				continue;

			if (m_InvenSlot[i][j])
				return false;
		}
	}
	return true;
}

CInvenItem::ITEM_DESC CInven_Manager::Setting_Item(_int iItemIndex, _int iItemGridX, _int iItemGridY)
{
	CInvenItem::ITEM_DESC Desc{};

	Desc.ItemData.iItemIndex = g_ItemData[iItemIndex].m_iItemID;
	Desc.ItemData.iInvenSizeX = g_ItemData[iItemIndex].m_iInvenSizeX;
	Desc.ItemData.iInvenSizeY = g_ItemData[iItemIndex].m_iInvenSizeY;
	Desc.vSize.x = _float(m_iSlotSize) * g_ItemData[iItemIndex].m_iSizeX;
	Desc.vSize.y = _float(m_iSlotSize) * g_ItemData[iItemIndex].m_iSizeY;
	Desc.vMinUV = g_ItemData[iItemIndex].m_vMinUV;
	Desc.vMaxUV = g_ItemData[iItemIndex].m_vMaxUV;
	Desc.ItemData.iItemGridX = iItemGridX;
	Desc.ItemData.iItemGridY = iItemGridY;
	Desc.ItemData.iItemCount = g_ItemData[iItemIndex].m_iItemValue;
	Desc.IsRender = m_IsInvenOpen;

	_float iCenterX = (iItemGridX + (iItemGridX + Desc.ItemData.iInvenSizeX - 1)) * 0.5f;
	_float iCenterY = (iItemGridY + (iItemGridY + Desc.ItemData.iInvenSizeY - 1)) * 0.5f;

	Convert_InvenWorld(iCenterX, iCenterY);

	Desc.vPos.x = iCenterX;
	Desc.vPos.y = iCenterY;

	return Desc;
}


void CInven_Manager::Convert_InvenGrid(_int& iX, _int& iY)
{
	_float fX = _float(iX - m_InvenWinPos.left) / m_iSlotSize;
	_float fY = _float(iY - m_InvenWinPos.top) / m_iSlotSize;

	iX = _int(round(fX));
	iY = _int(round(fY));
}

void CInven_Manager::Convert_InvenWorld(_float& iX, _float& iY)
{
	iX = (m_InvenWinPos.left + m_iSlotSize * 0.5f) + iX * m_iSlotSize;
	iY = (m_InvenWinPos.top + m_iSlotSize * 0.5f) + iY * m_iSlotSize;
}

void CInven_Manager::Free()
{
	__super::Free();
	for (auto Item : m_InvenItems)
	{
		Safe_Release(Item);
	}
	Safe_Release(m_pGameInstance);
}
