#pragma once
#include "Base.h"
#include "InvenItem.h"

NS_BEGIN(Client)
class CInven_Manager final : public CBase
{
	DECLARE_SINGLETON(CInven_Manager)
public:
	typedef struct Inventory_Desc {
		_float2	vInvenCenter = {};
		_uint	iSlotNumX = {};
		_uint	iSlotNumY = {};
		_uint	iSlotSize = {};

	}INVENTORY_DESC;

private:
	CInven_Manager();
	virtual ~CInven_Manager() = default;

public:
	HRESULT						Initialize(const INVENTORY_DESC& pDesc);
	HRESULT						Level_Init(_uint iLayerLevel, _wstring szLayerTag);
	_bool						Add_ItemSlot(_int iItemIndex, _wstring szPoolPath);
	_bool						Change_ItemSlot(CInvenItem* pItem);
	void						Add_Item(class CInvenItem* pItem);
	_bool						Erase_ItemSlot(class CInvenItem* pItem);

	void						InvenOpen(_bool IsOpen);
	const INVENTORY_DESC&		Get_InvenData();
	_bool						Reload_Check(_int iItemIndex );
	_bool						Bullet_Check(_int iItemIndex);
	_bool						IsItemCheck(_int i, class CInvenItem* pItem);

	_bool						Get_BulletCount(_int iItemIndex, _int& OutGunBullet, _int& OutInvenBullet);
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

	_bool						m_IsInvenOpen = { false };

	_int						m_iBullet[3] = {};

private:
	_bool						AddItem_Check(const _int& SizeX, const _int& SizeY, _int& iItemGridX, _int& iItemGridY);

	_bool						ItemSlot_Check(const _int& pSizeX, const _int& pSizeY, _int iStartX, _int iStartY);
	_bool						ItemSlot_Check(_int iStartX, _int iStartY, _int iSizeX, _int iSizeY, const INVEN_ITEM& Desc);

	CInvenItem::ITEM_DESC		Setting_Item(_int iItemIndex, _int iItemGridX, _int iItemGridY);

	void						Convert_InvenGrid(_int& iX, _int& iY);
	void						Convert_InvenWorld(_float& iX, _float& iY);

public:
	virtual void				Free() override;
};
NS_END