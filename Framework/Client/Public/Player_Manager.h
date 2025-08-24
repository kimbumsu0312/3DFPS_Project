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
	void			Set_PlayerPos(_vector vPos) { m_vPlayerPos = vPos;}

	void			Damage_On() { m_bIsDamage = false; }

	_vector			Get_PlayerPos() { return m_vPlayerPos; }
	_int			Get_Coin() { return m_iCoin; }
	const _int&		Get_Damage() { return m_iDamage; }
	void			Set_Damage(_int iDamage) { m_iDamage = iDamage; }
private:
	CGameInstance*	m_pGameInstance = { nullptr };
	_float			m_fMaxHp = {};
	_float			m_fPreHp = {};
	_float			m_fCulHp = {};
	_int			m_iCoin = {};
	_int			m_iDamage = {};
	_vector			m_vPlayerPos = {};

	_bool			m_bIsDamage = { false };
public:
	virtual void Free() override;
};
NS_END
