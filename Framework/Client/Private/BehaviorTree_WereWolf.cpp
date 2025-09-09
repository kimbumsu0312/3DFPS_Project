#include "pch.h"
#include "BehaviorTree_Alcina.h"
#include "Player_Manager.h"
#include "BehaviorTree_WereWolf.h"

CBehaviorTree_WereWolf::CBehaviorTree_WereWolf()
{
}

HRESULT CBehaviorTree_WereWolf::Initalize(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData)
{
	if (pData == nullptr)
		return E_FAIL;
	m_pBlackBoard = pData;
	Safe_AddRef(m_pBlackBoard);

	if (FAILED(Ready_Node()))
		return E_FAIL;

	return S_OK;
}

void CBehaviorTree_WereWolf::Update()
{
	if (m_pBlackBoard->Get_Data().iHp <= 0)
	{
		Switch_Die();
		return;
	}

	if (m_pBlackBoard->Get_Data().iDamage > 0)
	{
		Switch_Damage();
		return;
	}

	__super::Update();
}

HRESULT CBehaviorTree_WereWolf::Ready_Node()
{
	CSeletctorNode* pRoot = CSeletctorNode::Create();

	CSequenceNode* pEventSequence = CSequenceNode::Create();
	pRoot->Add_Node(pEventSequence);

	CActionNode* pEventCondition = CActionNode::Create([&]() {return Condition_Event_Check(); });
	CSeletctorNode* pEventSeletctor = CSeletctorNode::Create();
	pEventSequence->Add_Node(pEventCondition);
	pEventSequence->Add_Node(pEventSeletctor);

	CSequenceNode* pEvent3_Sequence = CSequenceNode::Create();
	CSequenceNode* pEvent2_Sequence = CSequenceNode::Create();
	CSequenceNode* pEvent1_Sequence = CSequenceNode::Create();

	pEventSeletctor->Add_Node(pEvent3_Sequence);
	pEventSeletctor->Add_Node(pEvent2_Sequence);
	pEventSeletctor->Add_Node(pEvent1_Sequence);

	CActionNode* pEvent3_Condition = CActionNode::Create([&]() {return Condition_Event_3(); });
	CActionNode* pEvent3_Switch = CActionNode::Create([&]() {return Switch_Event_3(); });
	pEvent3_Sequence->Add_Node(pEvent3_Condition);
	pEvent3_Sequence->Add_Node(pEvent3_Switch);
	
	CActionNode* pEvent2_Condition = CActionNode::Create([&]() {return Condition_Event_2(); });
	CActionNode* pEvent2_Switch = CActionNode::Create([&]() {return Switch_Event_2(); });
	pEvent2_Sequence->Add_Node(pEvent2_Condition);
	pEvent2_Sequence->Add_Node(pEvent2_Switch);
	
	CActionNode* pEvent1_Condition = CActionNode::Create([&]() {return Condition_Event_1(); });
	CActionNode* pEvent1_Switch = CActionNode::Create([&]() {return Switch_Event_1(); });
	pEvent1_Sequence->Add_Node(pEvent1_Condition);
	pEvent1_Sequence->Add_Node(pEvent1_Switch);

	CSeletctorNode* pChaseSeletor = CSeletctorNode::Create();
	pRoot->Add_Node(pChaseSeletor);

	CSequenceNode* pIdle_Sequence = CSequenceNode::Create();
	CSequenceNode* pAttack_Sequence = CSequenceNode::Create();
	CActionNode* pWalk_Switch = CActionNode::Create([&]() {return Switch_Walk(); });
	pChaseSeletor->Add_Node(pIdle_Sequence);
	pChaseSeletor->Add_Node(pAttack_Sequence);
	pChaseSeletor->Add_Node(pWalk_Switch);

	CActionNode* pIdle_Condition = CActionNode::Create([&]() {return Condition_Idle(); });
	CActionNode* pIdle_Switch = CActionNode::Create([&]() {return Switch_Idle(); });
	pIdle_Sequence->Add_Node(pIdle_Condition);
	pIdle_Sequence->Add_Node(pIdle_Switch);

	CActionNode* pAttack_Condition = CActionNode::Create([&]() {return Condition_Attack(); });
	CActionNode* pAttack_Switch = CActionNode::Create([&]() {return Switch_Attack(); });
	pAttack_Sequence->Add_Node(pAttack_Condition);
	pAttack_Sequence->Add_Node(pAttack_Switch);

	m_pRootNode = pRoot;
	return S_OK;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Die()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Die");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Damage()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Damage");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Condition_Event_Check()
{
	if (m_pBlackBoard->Get_Data().IsEvent_1 == true || m_pBlackBoard->Get_Data().IsEvent_2 == true || m_pBlackBoard->Get_Data().IsEvent_3 == true)
		return CNode::TREE_STATE::SUCCESS;
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Condition_Event_3()
{
	if(m_pBlackBoard->Get_Data().IsEvent_3 == true)
		return CNode::TREE_STATE::SUCCESS;
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Event_3()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event_3");

	if (m_pBlackBoard->Get_Data().IsEvent_3 == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Condition_Event_2()
{
	if (m_pBlackBoard->Get_Data().IsEvent_2 == true)
		return CNode::TREE_STATE::SUCCESS;
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Event_2()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event_2");

	if (m_pBlackBoard->Get_Data().IsEvent_2 == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Condition_Event_1()
{
	if (m_pBlackBoard->Get_Data().IsEvent_1 == true)
		return CNode::TREE_STATE::SUCCESS;
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Event_1()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event_1");

	if (m_pBlackBoard->Get_Data().IsEvent_1 == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Condition_Attack()
{
	if (m_pBlackBoard->Get_Data().fAttackCool > 0.f)
		return CNode::TREE_STATE::FAILED;

	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_vector vMonPos = { m_pBlackBoard->Get_Data().MonPos->m[3][0], m_pBlackBoard->Get_Data().MonPos->m[3][1], m_pBlackBoard->Get_Data().MonPos->m[3][2] };
	_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

	if (fDis <= 5.f)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		return CNode::TREE_STATE::SUCCESS;
	}
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Attack()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Attack");

	if (m_pBlackBoard->Get_Data().IsAttack == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}


CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Walk()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Walk");
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Condition_Idle()
{
	if (m_pBlackBoard->Get_Data().IsChase == false)
		return CNode::TREE_STATE::SUCCESS;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_WereWolf::Switch_Idle()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Idle");
	
	return CNode::TREE_STATE::SUCCESS;
}

CBehaviorTree_WereWolf* CBehaviorTree_WereWolf::Create(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData)
{
	CBehaviorTree_WereWolf* pInstance = new CBehaviorTree_WereWolf();

	if (FAILED(pInstance->Initalize(pData)))
	{
		MSG_BOX(TEXT("Failed to Create : CBehaviorTree_WereWolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBehaviorTree_WereWolf::Free()
{
	__super::Free();

	Safe_Release(m_pBlackBoard);
}
