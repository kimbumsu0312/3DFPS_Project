#pragma once
#include "Engine_Defines.h"

namespace Client
{
	enum class WEAPON_TYPE { PISTOL, SHOTGUN, SNIPER, END };
	enum class ITEM_TYPE { WEAPON, ITEM, END };
	enum class QUICKSLOT_TYPE { NONE, EQUIP, END };

	struct InvenItemData
	{
		_uint			m_iItemID;
		_wstring		m_szName;
		ITEM_TYPE		m_eType;

		_int			m_iInvenSizeX;
		_int			m_iInvenSizeY;
		_float			m_iSizeX;
		_float			m_iSizeY;
		_float2			m_vMinUV;
		_float2			m_vMaxUV;
		_int			m_iMaxItem;
		_int			m_iItemValue;
		QUICKSLOT_TYPE	m_eQuickSlot;
		_int			m_iQuickSlotType;
		_wstring		m_szInfo;

	public:
		InvenItemData(_uint iItemID, _wstring szName, ITEM_TYPE	eType, _int iInvenSizeX, _int iInvenSizeY, _float iSizeX, _float iSizeY,_float2 vMinUV, _float2 vMaxUV
						,_int iMaxItem, _int iItemValue, QUICKSLOT_TYPE eQuickType, _int iQuickSlotType, _wstring szInfo) :
			m_iItemID(iItemID), m_szName(szName), m_eType(eType), m_iInvenSizeX(iInvenSizeX), m_iInvenSizeY(iInvenSizeY), m_iSizeX(iSizeX), m_iSizeY(iSizeY), m_vMinUV(vMinUV), m_vMaxUV(vMaxUV), m_iMaxItem(iMaxItem)
			, m_iItemValue(iItemValue), m_eQuickSlot(eQuickType), m_iQuickSlotType(iQuickSlotType), m_szInfo(szInfo) {
		};
	};

	const vector<InvenItemData> g_ItemData =
	{
					//iItemID	,szName,						eType				iInvenSizeX, iInvenSizeY,	iSizeX,		iSizeY	vMinUV,			vMaxUV			iMaxItem, iItemValue
		InvenItemData(0			,TEXT("LEMI"),					ITEM_TYPE::WEAPON		,3			,2			,2			,2		,{12, 1257}		,{89, 1332}		,30			,0			,QUICKSLOT_TYPE::EQUIP			,0		,TEXT("Æò±ÕÀûÀÎ À§·ÂÀ» °¡Áø ÀÛ°í »ç¿ëÇÏ±â ½¬¿î ¹ÝÀÚµ¿ ÇÇ½ºÅç.")			),
		InvenItemData(1			,TEXT("M1897"),					ITEM_TYPE::WEAPON		,5			,2			,4			,2		,{188, 1258}	,{388, 1332}	,8			,0			,QUICKSLOT_TYPE::EQUIP			,1		,TEXT("ÁÖ·Î »ç³É¿¡ ¸¹ÀÌ ¾²ÀÌ´Â ÆßÇÁ ¿¬»ç½Ä »êÅºÃÑ.")			),
		InvenItemData(2			,TEXT("F2 ¼ÒÃÑ"),				ITEM_TYPE::WEAPON		,6			,2			,5			,2		,{765, 1258}	,{965, 1332}	,5			,0			,QUICKSLOT_TYPE::EQUIP			,1		,TEXT("¼öµ¿À¸·Î ³ë¸®¼è¸¦ ÀÚµ¿ÇÏ´Â Àú°Ý ¼ÒÃÑ.")		),
		InvenItemData(3			,TEXT("Ä®"),					ITEM_TYPE::WEAPON		,2			,2			,2			,2		,{0, 1351}		,{87, 1440}		,0			,0			,QUICKSLOT_TYPE::EQUIP			,0		,TEXT("Å« ³¯ÀÌ ´Þ¸° ¼Õ¶§ ¹¯Àº Ä®.")				),
		InvenItemData(4			,TEXT("È¸º¹¾à"),				ITEM_TYPE::ITEM			,1			,2			,1.2f		,1.5f	,{0, 0}			,{96, 96}		,1			,1			,QUICKSLOT_TYPE::EQUIP			,0		,TEXT("È¸º¹¾à")			),
		InvenItemData(5			,TEXT("±ÇÃÑ ÃÑ¾Ë"),				ITEM_TYPE::ITEM			,2			,1			,1.5f		,1.f	,{5, 215}		,{89, 259}		,30			,10			,QUICKSLOT_TYPE::NONE			,0		,TEXT("±ÇÃÑ ÃÑ¾Ë")		),
		InvenItemData(6			,TEXT("¼¦°Ç ÃÑ¾Ë"),				ITEM_TYPE::ITEM			,2			,1			,1.5f		,0.9f	,{103, 215}		,{189, 259}		,6			,3			,QUICKSLOT_TYPE::NONE			,0		,TEXT("¼¦°Ç ÃÑ¾Ë")		),
		InvenItemData(7			,TEXT("½º³ªÀÌÆÛ ÃÑ¾Ë"),			ITEM_TYPE::ITEM			,2			,1			,2			,0.9f	,{483, 215}		,{576, 259}		,12			,1			,QUICKSLOT_TYPE::NONE			,1		,TEXT("½º³ªÀÌÆÛ ÃÑ¾Ë")	),
	};	

	struct WorldItemData
	{
		_uint			m_iItemID;
		_wstring		m_szName;
		_wstring		m_szPoolPath;
		_wstring		m_szModelPath;

	public:
		WorldItemData(_uint iItemID, _wstring szName, _wstring szPoolPath, _wstring szModelPath) :
			m_iItemID(iItemID), m_szName(szName), m_szPoolPath(szPoolPath), m_szModelPath(szModelPath) {
		};
	};

	const vector<WorldItemData> g_3DItemData =
	{
		WorldItemData(0			,TEXT("LEMI")					,TEXT("Pool_HandGun")						,TEXT("Prototype_Model_HandGun")),
		WorldItemData(1			,TEXT("M1897")					,TEXT("Pool_ShotGun")						,TEXT("Prototype_Model_ShotGun")),
		WorldItemData(2			,TEXT("F2 ¼ÒÃÑ")				,TEXT("Pool_Sniper")						,TEXT("Prototype_Model_Sniper")),
		WorldItemData(3			,TEXT("Ä®")						,TEXT("Pool_Knife")							,TEXT("Prototype_Model_Knife")),
		WorldItemData(4			,TEXT("È¸º¹¾à")					,TEXT("Pool_Potion")						,TEXT("Prototype_Component_Model_Potion")),
		WorldItemData(5			,TEXT("±ÇÃÑ ÃÑ¾Ë")				,TEXT("Pool_Bullet_HandGun")				,TEXT("Prototype_Component_Model_Bullet_HandGun")),
		WorldItemData(6			,TEXT("¼¦°Ç ÃÑ¾Ë")				,TEXT("Pool_Bullet_ShotGun")				,TEXT("Prototype_Component_Model_Bullet_ShotGun")),
		WorldItemData(7			,TEXT("½º³ªÀÌÆÛ ÃÑ¾Ë")			,TEXT("Pool_Bullet_Sniper")					,TEXT("Prototype_Component_Model_Bullet_Sniper")),
	};
}