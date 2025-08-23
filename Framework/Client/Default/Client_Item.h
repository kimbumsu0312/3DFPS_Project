#pragma once
#include "Engine_Defines.h"

namespace Client
{
	enum class WEAPON_TYPE { PISTOL, SHOTGUN, SNIPER, END };
	enum class ITEM_TYPE { WEAPON, END };

	struct InvenItemData
	{
		_uint			m_iItemID;
		_int			m_iInvenSizeX;
		_int			m_iInvenSizeY;
		_float			m_iSizeX;
		_float			m_iSizeY;
		_float2			m_vMinUV;
		_float2			m_vMaxUV;
		_wstring		m_szName;

	public:
		InvenItemData(_uint iItemID, _wstring szName, _int iInvenSizeX, _int iInvenSizeY, _float iSizeX, _float iSizeY,_float2 vMinUV, _float2 vMaxUV) :
			m_iItemID(iItemID), m_szName(szName), m_iInvenSizeX(iInvenSizeX), m_iInvenSizeY(iInvenSizeY), m_iSizeX(iSizeX), m_iSizeY(iSizeY), m_vMinUV(vMinUV), m_vMaxUV(vMaxUV){
		};
	};

	const vector<InvenItemData> g_ItemData =
	{
		InvenItemData(0,TEXT("HandGun"), 3, 2, 2, 2, {12, 1257}, {89, 1332}),
		InvenItemData(1,TEXT("ShotGun"), 5, 2, 4, 2, {188, 1258}, {388, 1332}),
		InvenItemData(2,TEXT("Sniper"), 6, 2, 5, 2, {765, 1258}, {965, 1332}),
		InvenItemData(3,TEXT("Knife"), 2, 2, 2, 2, {0, 1351}, {87, 1440}),
		InvenItemData(4,TEXT("Potion"), 1, 2, 1.2f, 1.5f, {0, 0}, {96, 96}),
		InvenItemData(5,TEXT("HandGun_Bullet"), 2, 1, 1.5f, 1.f, {5, 215}, {89, 259}),
		InvenItemData(6,TEXT("ShotGun_Bullet"), 2, 1, 1.5f, 0.9f, {103, 215}, {189, 259}),
		InvenItemData(7,TEXT("Sniper_Bullet"), 2, 1, 2, 0.9f, {483, 215}, {576, 259}),
	};
}