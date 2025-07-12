#pragma once
#include "Engine_Defines.h"

namespace Client
{
	enum class WEAPON_TYPE { PISTOL, SHOTGUN, SNIPER, END };
	enum class ITEM_TYPE { WEAPON, END };
	
	struct ItemData
	{
		_uint			m_iItemID;
		_float4			m_vHudUV;
		_uint			m_vHudSizeType;
		_float4			m_vInvenUV;
		_float2			m_vInvenSize;
		_wstring		m_szName;
		_wstring		m_szDescription;
		ITEM_TYPE		m_eItemType = { ITEM_TYPE::END };
		WEAPON_TYPE		m_eWeaponType = { WEAPON_TYPE::END };
	public:
		ItemData(_uint iItemID, _float4 iHudUV, _uint vHudSize, _float4 iInvenUV, _float2 vInvenSize, _wstring szName,
			_wstring szDescription, ITEM_TYPE eItemType, WEAPON_TYPE eWeaponType) :
			m_iItemID(iItemID), m_vHudUV(iHudUV), m_vHudSizeType(vHudSize), m_vInvenUV(iInvenUV),
			m_vInvenSize(vInvenSize), m_szName(szName), m_szDescription(szDescription), m_eItemType(eItemType), m_eWeaponType(eWeaponType) {};
	};

	const vector<ItemData> g_ItemData =
	{
		ItemData(0,	{4, 87, 160, 105}, 1, {13, 1263, 89, 1328}, {2, 2}, L"Pistol", L"±ÇÃÑ",ITEM_TYPE::WEAPON, WEAPON_TYPE::PISTOL),
		ItemData(0,	{590, 22, 880, 92}, 2, {189, 1261, 385, 1320}, {3, 2}, L"Shotgun", L"¼¦±Ç",ITEM_TYPE::WEAPON, WEAPON_TYPE::SHOTGUN),

	};
}