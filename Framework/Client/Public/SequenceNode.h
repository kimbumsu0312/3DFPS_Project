#pragma once
#include "Node.h"

NS_BEGIN(Client)
class CSequenceNode final : public CNode
{
private:
	CSequenceNode();
	virtual ~CSequenceNode() = default;

public:
	virtual TREE_STATE		Evaluat() override;
	HRESULT					Add_Node(CNode* pBehaviorTree);
	virtual void			Reset() override;

private:
	vector<CNode*>			m_Children;

public:
	static CSequenceNode*		Create();
	virtual void				Free();
};

NS_END