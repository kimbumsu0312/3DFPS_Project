#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CItem_Manager final : public CBase
{
	DECLARE_SINGLETON(CItem_Manager)
private:
	CItem_Manager();
	virtual ~CItem_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_2DRenderGroup(const ITEM_2DRENDER_TYPE& eRenderType, _uint ItemIndex, _float2 vPos);

private:
	CGameInstance*					m_pGameInstance = { nullptr };
	vector<class CItem*>			m_ItemObjects[ENUM_CLASS(ITEM_2DRENDER_TYPE::END)];

public:
	virtual void Free() override;
};
NS_END