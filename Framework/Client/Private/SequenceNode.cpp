#include "pch.h"
#include "SequenceNode.h"

CSequenceNode::CSequenceNode()
{
}

CNode::TREE_STATE CSequenceNode::Evaluat()
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
			break;
		case CNode::TREE_STATE::FAILED:
			return eState;
		}
	}

	return CNode::TREE_STATE::SUCCESS;
}

HRESULT CSequenceNode::Add_Tree(CNode* pBehaviorTree)
{
	if (pBehaviorTree == nullptr)
		return E_FAIL;

	m_Children.push_back(pBehaviorTree);
	return S_OK;
}

CSequenceNode* CSequenceNode::Create()
{
	return new CSequenceNode();
}

void CSequenceNode::Free()
{
	__super::Free();
	for (auto Child : m_Children)
	{
		Safe_Release(Child);
	}

	m_Children.clear();
}
