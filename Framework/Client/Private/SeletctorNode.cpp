#include "pch.h"
#include "SeletctorNode.h"

CSeletctorNode::CSeletctorNode()
{
}

CNode::TREE_STATE CSeletctorNode::Evaluat()
{
	TREE_STATE eState = {};

	for (_int i = 0; i < m_Children.size(); ++i)
	{
		eState = m_Children[i]->Evaluat();
		switch (eState)
		{
		case CNode::TREE_STATE::RUN:
			m_iTreeIndex = i;
			return eState;
		case CNode::TREE_STATE::SUCCESS:
			return eState;
		case CNode::TREE_STATE::FAILED:
			break;
		}
	}
}

HRESULT CSeletctorNode::Add_Tree(CNode* pBehaviorTree)
{
	if (pBehaviorTree == nullptr)
		return E_FAIL;

	m_Children.push_back(pBehaviorTree);
	return S_OK;
}

CSeletctorNode* CSeletctorNode::Create()
{
	return new CSeletctorNode();
}

void CSeletctorNode::Free()
{
	__super::Free();
	for (auto Child : m_Children)
	{
		Safe_Release(Child);
	}

	m_Children.clear();
}
