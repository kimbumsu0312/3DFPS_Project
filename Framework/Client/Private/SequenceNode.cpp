#include "pch.h"
#include "SequenceNode.h"

CSequenceNode::CSequenceNode()
{
}

CNode::TREE_STATE CSequenceNode::Evaluat()
{
	TREE_STATE eState = {};

	for (;m_iTreeIndex < m_Children.size(); ++m_iTreeIndex)
	{
		eState = m_Children[m_iTreeIndex]->Evaluat();
		switch (eState)
		{
		case CNode::TREE_STATE::RUN:
			return eState;
		case CNode::TREE_STATE::SUCCESS:
			continue;
		case CNode::TREE_STATE::FAILED:
			m_iTreeIndex = 0;
			return eState;
		}
	}

	m_iTreeIndex = 0;
	return CNode::TREE_STATE::SUCCESS;
}

void CSequenceNode::Reset()
{
	__super::Reset();
	for (auto Child : m_Children)
		Child->Reset();

}

HRESULT CSequenceNode::Add_Node(CNode* pBehaviorTree)
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
