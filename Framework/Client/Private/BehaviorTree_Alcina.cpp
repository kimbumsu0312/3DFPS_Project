#include "pch.h"
#include "BehaviorTree_Alcina.h"
#include "Alcina.h"

CBehaviorTree_Alcina::CBehaviorTree_Alcina()
{
}

HRESULT CBehaviorTree_Alcina::Initalize(CBlackBoard<CAlcina::ALCHINA_DATA>* pData)
{
	if (pData == nullptr)
		return E_FAIL;
	m_pBlackBoard = pData;
	Safe_AddRef(m_pBlackBoard);
	
	if(FAILED(Ready_Node()))
		return E_FAIL;

	return S_OK;
}

void CBehaviorTree_Alcina::Update()
{
	__super::Update();
}

HRESULT CBehaviorTree_Alcina::Ready_Node()
{
	CSeletctorNode* pRoot = CSeletctorNode::Create();
	CSeletctorNode* pSeletctor = CSeletctorNode::Create();
	CSequenceNode* pSequence = CSequenceNode::Create();
	CActionNode* pAction = CActionNode::Create([&]() {return Test();});

	pSequence->Add_Tree(pAction);
	pSeletctor->Add_Tree(pSequence);
	pRoot->Add_Tree(pSeletctor);

	m_pRootNode = pRoot;

	m_pRootNode->Evaluat();
	return S_OK;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Test()
{
	return CNode::TREE_STATE::SUCCESS;
}

CBehaviorTree_Alcina* CBehaviorTree_Alcina::Create(CBlackBoard<CAlcina::ALCHINA_DATA>* pData)
{
	CBehaviorTree_Alcina* pInstance = new CBehaviorTree_Alcina();

	if (FAILED(pInstance->Initalize(pData)))
	{
		MSG_BOX(TEXT("Failed to Create : CBehaviorTree_Alcina"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBehaviorTree_Alcina::Free()
{
	__super::Free();
	Safe_Release(m_pBlackBoard);
}
