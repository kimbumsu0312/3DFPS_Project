#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CPlayerState abstract : public CStateObject<class CPlayer>
{
protected:
    CPlayerState();
    virtual ~CPlayerState() = default;

public:
    virtual HRESULT         Initalize(void* pArg = nullptr) = 0;
    virtual void            Enter(CPlayer* pContainer) = 0;
    virtual void            Update(CPlayer* pContainer, _float fTimeDelta) = 0;
    virtual void            Exit(CPlayer* pContainer) = 0;

protected:
    virtual void            Free() override;
};
NS_END