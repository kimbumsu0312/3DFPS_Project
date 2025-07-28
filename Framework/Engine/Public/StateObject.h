#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CStateObject abstract : public CBase
{
public:
        typedef struct tagState {
                _uint* pState = { nullptr };
        }STATE_DESC;
protected:
	CStateObject();
	virtual ~CStateObject() = default;

public:
    virtual HRESULT     Initalize(void *pArg);
    virtual void        Enter(class CContainerObject* pContainerObject) = 0;
    virtual void        Update(class CContainerObject* pContainerObject, _float fDeltatime) = 0;
    virtual void        Exit(class CContainerObject* pContainerObject) = 0;

protected:
    class CGameInstance*    m_pGameInstance = { nullptr };
    _uint*                  m_pState = { nullptr };
protected:
    virtual void            Free() override;
};
NS_END
