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
	void			Set_PlayerPos(_vector vPos) { m_vPlayerPos = vPos; }
	_vector			Get_PlayerPos() { return m_vPlayerPos; }
private:
	CGameInstance*	m_pGameInstance = { nullptr };
	_float			m_fMaxHp = {};
	_float			m_fPreHp = {};
	_float			m_fCulHp = {};

	_vector			m_vPlayerPos = {};
public:
	virtual void Free() override;
};
NS_END
