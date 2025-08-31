#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonState_Alcina abstract : public CStateObject<class CAlcina>
{
protected:
    CMonState_Alcina();
    virtual ~CMonState_Alcina() = default;

public:
    virtual HRESULT         Initalize(void* pArg = nullptr) = 0;
    virtual void            Enter(CAlcina* pContainer) = 0;
    virtual void            Update(CAlcina* pContainer, _float fTimeDelta) = 0;
    virtual void            Exit(CAlcina* pContainer) = 0;

protected:
    virtual void            Free() override;
};
NS_END