#pragma once
#include "Base.h"
#include "InvenItem.h"

NS_BEGIN(Client)
class CItemSpawner final : public CBase
{
	DECLARE_SINGLETON(CItemSpawner)

private:
	CItemSpawner();
	virtual ~CItemSpawner() = default;

public:
	HRESULT						Level_Init(_uint iLayerLevel, _wstring szLayerTag);
	HRESULT						Spawn_Item(_uint iItemIndex, _vector vPos, _int iNaviIndex);
private:
	CGameInstance*				m_pGameInstance = { nullptr };

	_uint						m_iLayerLevel = {};
	_wstring					m_szLayertag = {};

public:
	virtual void				Free() override;
};
NS_END