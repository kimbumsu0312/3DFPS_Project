#pragma once
#include "MonState.h"

NS_BEGIN(Client)
class CScouting_NorMon_1 final : public CMonState
{
private:
	enum class RELOAD_TYPE { BULLET, MAGAZINE, END };
private:
	CScouting_NorMon_1();
	virtual ~CScouting_NorMon_1() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(const NORMON_STATE& pMonState, CTransform* pTransformCom) override;
	virtual void				Update(_float fDeltatime, const NORMON_STATE& pMonState, CTransform* pTransformCom) override;
	virtual void				Exit() override;
private:
	STATE_ANIM					m_eAnimState = STATE_ANIM::END;
	RELOAD_TYPE					m_eReload_Type = RELOAD_TYPE::END;

public:
	static CScouting_NorMon_1* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
