#include "pch.h"
#include "ActionNode.h"

CActionNode::CActionNode() : CNode()
{
}

HRESULT CActionNode::Init_Action(function<TREE_STATE()> Action)
{
	m_Action = Action;
	return S_OK;
}

CNode::TREE_STATE CActionNode::Evaluat()
{
	return m_Action();
}

CActionNode* CActionNode::Create(function<TREE_STATE()> Action)
{
	CActionNode* pInstance = new CActionNode();

	if (FAILED(pInstance->Init_Action(Action)))
	{
		MSG_BOX(TEXT("Failed to Created : Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CActionNode::Free()
{
	__super::Free();
}
