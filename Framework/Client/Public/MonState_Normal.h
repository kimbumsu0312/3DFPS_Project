#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonState_Normal abstract : public CStateObject<class CMonster_Normal>
{
protected:
    CMonState_Normal();
    virtual ~CMonState_Normal() = default;

public:
    virtual HRESULT         Initalize(void* pArg = nullptr) = 0;
    virtual void            Enter(CMonster_Normal* pContainer) = 0;
    virtual void            Update(CMonster_Normal* pContainer, _float fTimeDelta) = 0;
    virtual void            Exit(CMonster_Normal* pContainer) = 0;

protected:
    virtual void            Free() override;
};
NS_END