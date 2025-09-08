#pragma once
#include "MonState_Normal.h"
#include "Monstate_WereWolf.h"
NS_BEGIN(Client)
class CDamage_WereWolf final : public CMonState_WereWolf
{
private:
	CDamage_WereWolf();
	virtual ~CDamage_WereWolf() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CMonster_WereWolf* pContainer) override;
	virtual void				Update(CMonster_WereWolf* pContainer, _float fDeltatime) override;
	virtual void				Exit(CMonster_WereWolf* pContainer) override;

public:
	static CDamage_WereWolf* Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
