#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGarbage_Collector final : public CBase
{
private:
	CGarbage_Collector();
	virtual ~CGarbage_Collector() = default;

public:
	void						GarbageSweep(class CGameObject* pObject);
	void						Clear_Garbage();

private:
	queue<class CGameObject*>	m_Garbage;

public:
	static CGarbage_Collector*	Create();
	virtual void				Free();
};
NS_END