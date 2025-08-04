#pragma once
#include "MonState.h"

NS_BEGIN(Client)
class CStand_NorMon_1 final : public CMonState
{
private:
	CStand_NorMon_1();
	virtual ~CStand_NorMon_1() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(const NORMON_STATE& pMonState) override;
	virtual void				Update(_float fDeltatime, const NORMON_STATE& pMonState) override;
	virtual void				Exit() override;
private:
	STATE_ANIM					m_eAnimState = { STATE_ANIM::END };

public:
	static CStand_NorMon_1*		Create(void* pArg);
	virtual void				Free() override;
};
NS_END
