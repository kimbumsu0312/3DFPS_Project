#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonState_Daniela abstract : public CStateObject<class CDaniela>
{
protected:
    CMonState_Daniela();
    virtual ~CMonState_Daniela() = default;

public:
    virtual HRESULT         Initalize(void* pArg = nullptr) = 0;
    virtual void            Enter(CDaniela* pContainer) = 0;
    virtual void            Update(CDaniela* pContainer, _float fTimeDelta) = 0;
    virtual void            Exit(CDaniela* pContainer) = 0;

protected:
    virtual void            Free() override;
};
NS_END