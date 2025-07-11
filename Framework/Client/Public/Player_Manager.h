#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CPlayer_Manager final : public CBase
{
	DECLARE_SINGLETON(CPlayer_Manager)
private:
	CPlayer_Manager();
	virtual ~CPlayer_Manager() = default;

public:
	HRESULT Initialize();
	
private:

public:
	virtual void Free() override;
};
NS_END
