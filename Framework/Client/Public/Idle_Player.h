#pragma once
#include "StateObject.h"

NS_BEGIN(Client)
class CIdle_Player final : public CStateObject
{
private:
	CIdle_Player();
	virtual ~CIdle_Player() = default;

public:
	virtual HRESULT				Initalize(void* pArg) override;
	virtual void				Enter(class CContainerObject* pContainerObject) override;
	virtual void				Update(class CContainerObject* pContainerObject, _float fDeltatime) override;
	virtual void				Exit(class CContainerObject* pContainerObject) override;

public:
	static CIdle_Player*		Create(void* pArg);
	virtual void				Free() override;
};
NS_END
