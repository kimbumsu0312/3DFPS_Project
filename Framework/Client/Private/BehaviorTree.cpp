#include "pch.h"
#include "BehaviorTree.h"

CBehaviorTree::CBehaviorTree()
{
}

HRESULT CBehaviorTree::Initalize(void* pArg)
{
	return S_OK;
}

void CBehaviorTree::Update()
{
	if (m_pRootNode == nullptr)
		return;

	m_pRootNode->Evaluat();
}

void CBehaviorTree::Free()
{
	Safe_Release(m_pRootNode);
	__super::Free();
}
