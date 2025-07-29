#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CStateObject abstract : public CBase
{
public:
        typedef struct tagState {
                _uint*      pState = { nullptr };
                _uint*      pWeaponState = { nullptr };
                string*     pAnimTag = { nullptr };
                _wstring*   pStateTag = { nullptr };
                _bool*      pIsAnimLoop = { nullptr };
                _bool*      pIsAnimFinsh = { nullptr };
        }STATE_DESC;
protected:
	CStateObject();
	virtual ~CStateObject() = default;

public:
    virtual HRESULT         Initalize(void *pArg);
    virtual void            Enter() = 0;
    virtual void            Update(_float fDeltatime) = 0;
    virtual void            Exit() = 0;

protected:
    CGameInstance*          m_pGameInstance = { nullptr };
    _uint*                  m_pState = { nullptr };
    _uint*                  m_pWeaponState = { nullptr };
    string*                 m_pAnimTag = { nullptr };
    _wstring*               m_pStateTag = { nullptr };
    _bool*					m_pIsAnimLoop = { nullptr };
    _bool*					m_pIsAnimFinsh = { nullptr };

protected:
    virtual void            Free() override;
};
NS_END
