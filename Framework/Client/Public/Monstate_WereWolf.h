#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonState_WereWolf abstract : public CStateObject<class CMonster_WereWolf>
{
protected:
    CMonState_WereWolf();
    virtual ~CMonState_WereWolf() = default;

public:
    virtual HRESULT         Initalize(void* pArg = nullptr) = 0;
    virtual void            Enter(CMonster_WereWolf* pContainer) = 0;
    virtual void            Update(CMonster_WereWolf* pContainer, _float fTimeDelta) = 0;
    virtual void            Exit(CMonster_WereWolf* pContainer) = 0;

protected:
    virtual void            Free() override;
};
NS_END