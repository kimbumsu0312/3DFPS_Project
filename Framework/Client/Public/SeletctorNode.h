#pragma once
#include "Node.h"

NS_BEGIN(Client)
class CSeletctorNode final : public CNode
{
private:
	CSeletctorNode();
	virtual ~CSeletctorNode() = default;

public:
	virtual TREE_STATE			Evaluat() override;
	HRESULT						Add_Node(CNode* pBehaviorTree);
	virtual void				Reset() override;

private:
	vector<CNode*>				m_Children;

public:
	static CSeletctorNode*			Create();
	virtual void				Free() override;
};
NS_END
