#pragma once
#include "Base.h"
#include "Client_Struct.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CStateObject abstract : public CBase
{
protected:
	CStateObject();
	virtual ~CStateObject() = default;

public:
    virtual HRESULT         Initalize(void *pArg);
    virtual void            Enter() {};    
    virtual void            Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) {};
    virtual void            Enter(const NORMON_STATE& pMonState, CTransform* pTransformCom) {};
    virtual void            Update(_float fDeltatime) {};
    virtual void            Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) {};
    virtual void            Update(_float fDeltatime, const NORMON_STATE& pMonState, CTransform* pTransformCom) {};
    virtual void            Exit() {};

protected:
    CGameInstance*          m_pGameInstance = { nullptr };

protected:
    virtual void            Free() override;
};
NS_END
