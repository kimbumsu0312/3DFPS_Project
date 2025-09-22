#include "pch.h"
#include "BehaviorTree_Bela.h"
#include "Bela.h"
#include "Player_Manager.h"

CBehaviorTree_Bela::CBehaviorTree_Bela()
{
}

HRESULT CBehaviorTree_Bela::Initalize(CBlackBoard<CBela::BELA_DATA>* pData)
{
	if (pData == nullptr)
		return E_FAIL;
	m_pBlackBoard = pData;
	Safe_AddRef(m_pBlackBoard);

	if (FAILED(Ready_Node()))
		return E_FAIL;

	return S_OK;
}

void CBehaviorTree_Bela::Update()
{
	if (m_pBlackBoard->Get_Data().iHp <= 0)
	{
		Switch_Die();
		return;
	}

	__super::Update();
}

HRESULT CBehaviorTree_Bela::Ready_Node()
{
	CSeletctorNode* pRoot = CSeletctorNode::Create();

	CSequenceNode* pEventSequence = CSequenceNode::Create();
	pRoot->Add_Node(pEventSequence);

	CActionNode* pEventCondition = CActionNode::Create([&]() {return Condition_Event_Check(); });
	CSeletctorNode* pEventSeletctor = CSeletctorNode::Create();
	pEventSequence->Add_Node(pEventCondition);
	pEventSequence->Add_Node(pEventSeletctor);

	CActionNode* pEvent3_Switch = CActionNode::Create([&]() {return Switch_Event_3(); });
	CActionNode* pEvent2_Switch = CActionNode::Create([&]() {return Switch_Event_2(); });
	CActionNode* pEvent1_Switch = CActionNode::Create([&]() {return Switch_Event_1(); });
	CActionNode* pSpawn_Switch = CActionNode::Create([&]() {return Switch_Spawn(); });

	pEventSeletctor->Add_Node(pEvent3_Switch);
	pEventSeletctor->Add_Node(pEvent2_Switch);
	pEventSeletctor->Add_Node(pEvent1_Switch);
	pEventSeletctor->Add_Node(pSpawn_Switch);

	CSeletctorNode* pChaseSeletor = CSeletctorNode::Create();
	pRoot->Add_Node(pChaseSeletor);

	CSequenceNode* pWalk_Sequence = CSequenceNode::Create();
	CActionNode* pIdle_Switch = CActionNode::Create([&]() {return Switch_Idle(); });
	pChaseSeletor->Add_Node(pWalk_Sequence);
	pChaseSeletor->Add_Node(pIdle_Switch);

	CActionNode* pWalk_Condition = CActionNode::Create([&]() {return Condition_Walk(); });
	CActionNode* pWalk_Switch = CActionNode::Create([&]() {return Switch_Walk(); });
	pWalk_Sequence->Add_Node(pWalk_Condition);
	pWalk_Sequence->Add_Node(pWalk_Switch);

	m_pRootNode = pRoot;

	return S_OK;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Die()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Die");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Bela::Condition_Event_Check()
{
	if (m_pBlackBoard->Get_Data().IsEvent_1 == true || m_pBlackBoard->Get_Data().IsEvent_2 == true 
		|| m_pBlackBoard->Get_Data().IsEvent_3 == true || m_pBlackBoard->Get_Data().IsSpawn == true)
		return CNode::TREE_STATE::SUCCESS;
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Event_3()
{
	if (m_pBlackBoard->Get_Data().IsEvent_3 == true)
	{
		*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event_3");
		return CNode::TREE_STATE::RUN;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Event_2()
{
	if (m_pBlackBoard->Get_Data().IsEvent_2 == true)
	{
		*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event_2");
		return CNode::TREE_STATE::RUN;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Event_1()
{
	if (m_pBlackBoard->Get_Data().IsEvent_1 == true)
	{
		*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event_1");
		return CNode::TREE_STATE::RUN;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Spawn()
{
	if (m_pBlackBoard->Get_Data().IsSpawn == true)
	{
		*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Spawn");
		return CNode::TREE_STATE::RUN;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Bela::Condition_Walk()
{
	if (m_pBlackBoard->Get_Data().IsChase == true)
		return CNode::TREE_STATE::SUCCESS;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Walk()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Walk");

	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Bela::Switch_Idle()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Idle");

	return CNode::TREE_STATE::SUCCESS;
}

CBehaviorTree_Bela* CBehaviorTree_Bela::Create(CBlackBoard<CBela::BELA_DATA>* pData)
{
	CBehaviorTree_Bela* pInstance = new CBehaviorTree_Bela();

	if (FAILED(pInstance->Initalize(pData)))
	{
		MSG_BOX(TEXT("Failed to Create : CBehaviorTree_Bela"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBehaviorTree_Bela::Free()
{
	__super::Free();

	Safe_Release(m_pBlackBoard);
}
