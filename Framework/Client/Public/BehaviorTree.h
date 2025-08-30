#pragma once
#include "Base.h"
#include "SeletctorNode.h"
#include "ActionNode.h"
#include "SequenceNode.h"

NS_BEGIN(Client)
class CBehaviorTree abstract : public CBase
{
protected:
	CBehaviorTree();
	virtual ~CBehaviorTree() = default;

public:
	virtual HRESULT			Initalize(void* pArg);
	virtual void			Update();

protected:
	CNode*					m_pRootNode = { nullptr };

public:
	virtual void			Free();
};

NS_END

