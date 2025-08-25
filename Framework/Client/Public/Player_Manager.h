#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CPlayer_Manager final : public CBase
{
	DECLARE_SINGLETON(CPlayer_Manager)

private:
	enum class QUICKSLOT { QUICK_1, QUICK_2, QUICK_3, QUICK_4, END};
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
	void			Add_QuickSlotItem(_int iSlotIndex, _int iItemIndex);
	void			Selete_Slot(_int i);
private:
	CGameInstance*	m_pGameInstance = { nullptr };
	_float			m_fMaxHp = {};
	_float			m_fPreHp = {};
	_float			m_fCulHp = {};
	_int			m_iCoin = {};
	_int			m_iDamage = {};
	_vector			m_vPlayerPos = {};

	_int			m_iQuickSlot[ENUM_CLASS(QUICKSLOT::END)] = { -1};
	_int			m_iPreSeleteItem = { 0 };
	_int			m_iSeleteItem = { 0 };

	_bool			m_bIsDamage = { false };
public:
	virtual void Free() override;
};
NS_END
