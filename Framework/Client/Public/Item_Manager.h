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

private:
	CGameInstance*					m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};
NS_END