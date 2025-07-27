#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CStateObject abstract : public CBase
{
protected:
	CStateObject();
	virtual ~CStateObject() = default;

public:
    virtual void        Enter(class CContainerObject* pContainerObject) = 0;
    virtual void        Update(class CContainerObject* pContainerObject, _float fDeltatime) = 0;
    virtual void        Exit(class CContainerObject* pContainerObject) = 0;

protected:
    class CGameInstance*    m_pGameInstance = { nullptr };


protected:
    virtual void            Free() override;
};
NS_END
