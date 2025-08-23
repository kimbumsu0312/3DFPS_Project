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
		_float2			m_vSize;
		_float2			m_vMinUV;
		_float2			m_vMaxUV;

	public:
		InvenItemData(_uint iItemID, _int iInvenSizeX, _int iInvenSizeY, _float2 vSize, _float2 vMinUV, _float2 vMaxUV) :
			m_iItemID(iItemID), m_iInvenSizeX(iInvenSizeX), m_iInvenSizeY(iInvenSizeY), m_vSize(vSize), m_vMinUV(vMinUV), m_vMaxUV(vMaxUV){
		};
	};

	const vector<InvenItemData> g_ItemData =
	{
		InvenItemData(0, 1, 1, { 0, 0 }, {0, 0}, {1, 1}),
		InvenItemData(1, 2, 2, { 0, 0 }, {0, 0}, {1, 1}),
		InvenItemData(2, 3, 3, { 0, 0 }, {0, 0}, {1, 1}),
		InvenItemData(3, 3, 2, { 0, 0 }, {0, 0}, {1, 1}),
		InvenItemData(4, 2, 4, { 0, 0 }, {0, 0}, {1, 1}),

	};
}