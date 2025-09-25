#pragma once
#include "MonState_Alcina.h"

NS_BEGIN(Client)
class CDamage_Alcina final : public CMonState_Alcina
{
private:
	enum class Damage_Type { LOW, MIDDIE, HIGHT, END};
private:
	CDamage_Alcina();
	virtual ~CDamage_Alcina() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CAlcina* pContainer) override;
	virtual void				Update(CAlcina* pContainer, _float fDeltatime) override;
	virtual void				Exit(CAlcina* pContainer) override;

private:
	_bool						m_isNoies = { true };
private:
	void						Damage_F(CAlcina* pContainer);

public:
	static CDamage_Alcina* Create(void* pArg = nullptr);
	virtual void				Free() override;
};

NS_END