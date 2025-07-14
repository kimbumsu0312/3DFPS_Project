#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CPlayer_Manager final : public CBase
{
	DECLARE_SINGLETON(CPlayer_Manager)
private:
	CPlayer_Manager();
	virtual ~CPlayer_Manager() = default;

public:
	HRESULT			Initialize();
	void			Player_Hp(_int iValue);

private:
	CGameInstance*	m_pGameInstance = { nullptr };
	_float			m_fMaxHp = {};
	_float			m_fPreHp = {};
	_float			m_fCulHp = {};

public:
	virtual void Free() override;
};
NS_END
