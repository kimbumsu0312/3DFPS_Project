#include "pch.h"
#include "SeletctorNode.h"

CSeletctorNode::CSeletctorNode()
{
}

CNode::TREE_STATE CSeletctorNode::Evaluat()
{
	if (m_Children.empty())
		return TREE_STATE::FAILED;


	for (; m_iTreeIndex < m_Children.size(); ++m_iTreeIndex)
	{
		if (m_Children[m_iTreeIndex] == nullptr) 
			continue;

		TREE_STATE eState = {};

		eState = m_Children[m_iTreeIndex]->Evaluat();
		switch (eState)
		{
		case CNode::TREE_STATE::RUN:
			return CNode::TREE_STATE::RUN;
		case CNode::TREE_STATE::SUCCESS:
			m_iTreeIndex = 0;
			return CNode::TREE_STATE::SUCCESS;
		case CNode::TREE_STATE::FAILED:
			continue;
		}
	}
	m_iTreeIndex = 0; 
	return CNode::TREE_STATE::FAILED;
}

HRESULT CSeletctorNode::Add_Node(CNode* pBehaviorTree)
{
	if (pBehaviorTree == nullptr)
		return E_FAIL;

	m_Children.push_back(pBehaviorTree);
	return S_OK;
}

void CSeletctorNode::Reset()
{
	__super::Reset();
	for (auto Child : m_Children)
		Child->Reset();
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
