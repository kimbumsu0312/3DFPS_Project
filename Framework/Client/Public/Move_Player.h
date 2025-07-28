#pragma once
#include "StateObject.h"

NS_BEGIN(Client)
class CMove_Player final : public CStateObject
{
public:
	typedef struct tagMovePlayerState : public CStateObject::STATE_DESC{
		CTransform*			pTransForm = { nullptr };
	}MOVE_PLAYER_DESC;

private:
	CMove_Player();
	virtual ~CMove_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(class CContainerObject* pContainerObject) override;
	virtual void				Update(class CContainerObject* pContainerObject, _float fDeltatime) override;
	virtual void				Exit(class CContainerObject* pContainerObject) override;

private:
	CTransform*					m_pTransForm = { nullptr };
public:
	static CMove_Player* Create(void* pArg);
	virtual void				Free() override;
};
NS_END
