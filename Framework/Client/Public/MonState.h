#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonState abstract : public CStateObject
{
public:
    typedef struct tagMonStateDesc {
        _uint*      pAnimState = { nullptr };
        string*     pAnimTag = { nullptr };
        _wstring*   pStateTag = { nullptr };
        _bool*      pIsAnimLoop = { nullptr };
        _bool*      pIsAnimFinsh = { nullptr };
    }Mon_STATE_DESC;
protected:
    CMonState();
    virtual ~CMonState() = default;

public:
    virtual HRESULT         Initalize(void* pArg);
    virtual void            Enter(const NORMON_STATE& pMonState) = 0;
    virtual void            Update(_float fDeltatime, const NORMON_STATE& pMonState) = 0;
    virtual void            Exit() = 0;

protected:
    _uint*                  m_pAnimState = { nullptr };
    string*                 m_pAnimTag = { nullptr };
    _wstring*               m_pStateTag = { nullptr };
    _bool*                  m_pIsAnimLoop = { nullptr };
    _bool*                  m_pIsAnimFinsh = { nullptr };

protected:
    virtual void            Free() override;
};
NS_END