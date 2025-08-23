#pragma once
#include "Base.h"
#include "InvenItem.h"

NS_BEGIN(Client)
class CInventory_Manager final : public CBase
{
	DECLARE_SINGLETON(CInventory_Manager)
public:
	typedef struct Inventory_Desc {
		_float2	vInvenCenter = {};
		_uint	iSlotNumX = {};
		_uint	iSlotNumY = {};
		_uint	iSlotSize = {};

	}INVENTORY_DESC;

private:
	CInventory_Manager();
	virtual ~CInventory_Manager() = default;

public:
	HRESULT						Initialize(const INVENTORY_DESC& pDesc);
	HRESULT						Level_Init(_uint iLayerLevel, _wstring szLayerTag);
	_bool						Add_ItemSlot(_int iItemIndex, _wstring szPoolPath);
	void						Add_Item(class CInvenItem* pItem);
	_bool						Erase_ItemSlot(class CInvenItem* pItem);

private:
	CGameInstance*				m_pGameInstance = { nullptr };

	vector<class CInvenItem*>	m_InvenItems;
	vector<vector<_bool>>		m_InvenSlot;

	_uint						m_iLayerLevel = {};
	_wstring					m_szLayertag = {};

	_float2						m_vInvenCenter = {};
	INVENRECT					m_InvenWinPos = {};
	INVENRECT					m_InvenWinSize = {};

	_uint						m_iSlotNumX = {};
	_uint						m_iSlotNumY = {};
	_uint						m_iSlotSize = {};

private:
	_bool						AddItem_Check(const _int& SizeX, const _int& SizeY, _int& iItemGridX, _int& iItemGridY);
	_bool						ItemSlot_Check(const _int& pSizeX, const _int& pSizeY, _int iStartX, _int iStartY);
	
	CInvenItem::ITEM_DESC		Setting_Item(_int iItemIndex, _int iItemGridX, _int iItemGridY);

	void						Convert_InvenGrid(_int& iX, _int& iY);
	void						Convert_InvenWorld(_int& iX, _int& iY);

public:
	virtual void				Free() override;
};
NS_END