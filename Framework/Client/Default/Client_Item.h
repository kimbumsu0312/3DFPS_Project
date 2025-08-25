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
		InvenItemData(0			,TEXT("LEMI"),					ITEM_TYPE::WEAPON		,3			,2			,2			,2		,{12, 1257}		,{89, 1332}		,30			,0			,QUICKSLOT_TYPE::EQUIP			,0		,TEXT("평균적인 위력을 가진 작고 사용하기 쉬운 반자동 피스톨.")			),
		InvenItemData(1			,TEXT("M1897"),					ITEM_TYPE::WEAPON		,5			,2			,4			,2		,{188, 1258}	,{388, 1332}	,8			,0			,QUICKSLOT_TYPE::EQUIP			,1		,TEXT("주로 사냥에 많이 쓰이는 펌프 연사식 산탄총.")			),
		InvenItemData(2			,TEXT("F2 소총"),				ITEM_TYPE::WEAPON		,6			,2			,5			,2		,{765, 1258}	,{965, 1332}	,5			,0			,QUICKSLOT_TYPE::EQUIP			,1		,TEXT("수동으로 노리쇠를 자동하는 저격 소총.")		),
		InvenItemData(3			,TEXT("칼"),					ITEM_TYPE::WEAPON		,2			,2			,2			,2		,{0, 1351}		,{87, 1440}		,0			,0			,QUICKSLOT_TYPE::EQUIP			,0		,TEXT("큰 날이 달린 손때 묻은 칼.")				),
		InvenItemData(4			,TEXT("회복약"),				ITEM_TYPE::ITEM			,1			,2			,1.2f		,1.5f	,{0, 0}			,{96, 96}		,1			,1			,QUICKSLOT_TYPE::EQUIP			,0		,TEXT("회복약")			),
		InvenItemData(5			,TEXT("권총 총알"),				ITEM_TYPE::ITEM			,2			,1			,1.5f		,1.f	,{5, 215}		,{89, 259}		,30			,10			,QUICKSLOT_TYPE::NONE			,0		,TEXT("권총 총알")		),
		InvenItemData(6			,TEXT("샷건 총알"),				ITEM_TYPE::ITEM			,2			,1			,1.5f		,0.9f	,{103, 215}		,{189, 259}		,6			,3			,QUICKSLOT_TYPE::NONE			,0		,TEXT("샷건 총알")		),
		InvenItemData(7			,TEXT("스나이퍼 총알"),			ITEM_TYPE::ITEM			,2			,1			,2			,0.9f	,{483, 215}		,{576, 259}		,12			,1			,QUICKSLOT_TYPE::NONE			,1		,TEXT("스나이퍼 총알")	),
	};	
}