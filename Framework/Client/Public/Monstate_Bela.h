#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonState_Bela abstract : public CStateObject<class CBela>
{
protected:
    CMonState_Bela();
    virtual ~CMonState_Bela() = default;

public:
    virtual HRESULT         Initalize(void* pArg = nullptr) = 0;
    virtual void            Enter(CBela* pContainer) = 0;
    virtual void            Update(CBela* pContainer, _float fTimeDelta) = 0;
    virtual void            Exit(CBela* pContainer) = 0;

protected:
    CGameInstance*          m_pGameInstance = { nullptr};
protected:
    virtual void            Free() override;
};
NS_END