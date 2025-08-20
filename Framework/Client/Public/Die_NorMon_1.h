#pragma once
#include "MonState_Normal.h"

NS_BEGIN(Client)
class CDie_Normon_1 final : public CMonState_Normal
{
private:
	CDie_Normon_1();
	virtual ~CDie_Normon_1() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(CMonster_Normal* pContainer) override;
	virtual void				Update(CMonster_Normal* pContainer, _float fDeltatime) override;
	virtual void				Exit(CMonster_Normal* pContainer) override;

public:
	static CDie_Normon_1* Create(void* pArg = nullptr);
	virtual void				Free() override;
};
NS_END
