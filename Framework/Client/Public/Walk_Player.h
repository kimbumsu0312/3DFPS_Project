#pragma once
#include "StateObject.h"

NS_BEGIN(Enigne)
class CTransform;
NS_END

NS_BEGIN(Client)
class CWalk_Player final : public CStateObject
{
private:
	enum class MOVE_SATATE { F, R, L, B, SF, SR, SL, SB, END};

private:
	CWalk_Player();
	virtual ~CWalk_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter() override;
	virtual void				Update(_float fDeltatime) override;
	virtual void				Exit() override;

private:
	MOVE_SATATE					m_eMoveKey = { MOVE_SATATE::END };
private:
	void						AnimSet();
	void						KeyInput(_float fTimeDelta);

public:
	static CWalk_Player* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
