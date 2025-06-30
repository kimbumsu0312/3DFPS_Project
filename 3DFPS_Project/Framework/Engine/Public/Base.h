#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	unsigned int AddRef();
	unsigned int Release();

private:
	unsigned int m_iRefCnt = {};

public:
	virtual void Free();
};
NS_END



