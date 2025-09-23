#include "pch.h"
#include "BehaviorTree_Alcina.h"
#include "Alcina.h"
#include "Player_Manager.h"

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

	m_iMaxHp = pData->Get_Data().iHp;
	m_isPage2 = false;
	return S_OK;
}

void CBehaviorTree_Alcina::Update()
{
	if (m_pBlackBoard->Get_Data().iHp <= 0)
	{
		Switch_Die();
		m_pRootNode->Reset();
		return;
	}

	if (m_pBlackBoard->Get_Data().iDamage > 0)
	{
		Switch_Damage();
		m_pRootNode->Reset();
		return;
	}

	if (!m_isPage2 && m_pBlackBoard->Get_Data().iHp < m_iMaxHp * 0.5f)
	{
		m_isPage2 = true;
		m_pBlackBoard->Set_Data().IsEvent_1 = true;
		m_pBlackBoard->Set_Data().IsPage2 = true;
	}

	__super::Update();
}

HRESULT CBehaviorTree_Alcina::Ready_Node()
{
	CSeletctorNode* pRoot = CSeletctorNode::Create();

	CSequenceNode* pEventSequence = CSequenceNode::Create();
	pRoot->Add_Node(pEventSequence);

	CActionNode* pEventCondition = CActionNode::Create([&]() {return Condition_Event_Check(); });
	CSeletctorNode* pEventSeletctor = CSeletctorNode::Create();
	pEventSequence->Add_Node(pEventCondition);
	pEventSequence->Add_Node(pEventSeletctor);

	CActionNode* pEvent1_Switch = CActionNode::Create([&]() {return Switch_Event_1(); });

	pEventSeletctor->Add_Node(pEvent1_Switch);

	CSequenceNode* pAttack2Sequence = CSequenceNode::Create();
	CActionNode* pAttack2Condition = CActionNode::Create([&]() {return Condition_Attack2(); });
	CActionNode* pAttack2Switch = CActionNode::Create([&]() {return Switch_Attack2(); });
	pAttack2Sequence->Add_Node(pAttack2Condition);
	pAttack2Sequence->Add_Node(pAttack2Switch);

	CSeletctorNode* pDefultAttack = CSeletctorNode::Create();
	CSequenceNode* pAttack3Sequence = CSequenceNode::Create();
	CSequenceNode* pAttack1Sequence = CSequenceNode::Create();

	pDefultAttack->Add_Node(pAttack3Sequence);
	pDefultAttack->Add_Node(pAttack1Sequence);

	CActionNode* pAttack3Condition = CActionNode::Create([&]() {return Condition_Attack3(); });
	CActionNode* pAttack3Switch = CActionNode::Create([&]() {return Switch_Attack3(); });
	pAttack3Sequence->Add_Node(pAttack3Condition);
	pAttack3Sequence->Add_Node(pAttack3Switch);

	CActionNode* pAttack1Condition = CActionNode::Create([&]() {return Condition_Attack1(); });
	CActionNode* pAttack1Switch = CActionNode::Create([&]() {return Switch_Attack1(); });
	pAttack1Sequence->Add_Node(pAttack1Condition);
	pAttack1Sequence->Add_Node(pAttack1Switch);

	CSequenceNode* pWalkSequence = CSequenceNode::Create();
	CActionNode* pWalkCondition = CActionNode::Create([&]() {return Condition_Walk(); });
	CActionNode* pWalkSwitch = CActionNode::Create([&]() {return Switch_Walk(); });
	pWalkSequence->Add_Node(pWalkCondition);
	pWalkSequence->Add_Node(pWalkSwitch);

	CActionNode* pIdleSwitch = CActionNode::Create([&]() {return Switch_Idle(); });

	pRoot->Add_Node(pAttack2Sequence);
	pRoot->Add_Node(pDefultAttack);
	pRoot->Add_Node(pWalkSequence);
	pRoot->Add_Node(pIdleSwitch);

	m_pRootNode = pRoot;
	return S_OK;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Die()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Die");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Damage()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Damage");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Condition_Event_Check()
{
	if (m_pBlackBoard->Get_Data().IsEvent_1 == true)
		return CNode::TREE_STATE::SUCCESS;
	else
		return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Event_1()
{
	if (m_pBlackBoard->Get_Data().IsEvent_1 == true)
	{
		*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Event1");
		return CNode::TREE_STATE::RUN;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Condition_Attack3()
{
	if (m_pBlackBoard->Get_Data().IsChase == false)
		return CNode::TREE_STATE::FAILED;

	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_vector vMonPos = { m_pBlackBoard->Get_Data().MonPos->m[3][0], m_pBlackBoard->Get_Data().MonPos->m[3][1], m_pBlackBoard->Get_Data().MonPos->m[3][2] };
	_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

	if (fDis <= 4.5f && m_isPage2 == true)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		return CNode::TREE_STATE::SUCCESS;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Attack3()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Attack3");

	if (m_pBlackBoard->Get_Data().IsAttack == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Condition_Attack2()
{
	if (m_pBlackBoard->Get_Data().IsChase == false)
		return CNode::TREE_STATE::FAILED;

	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_vector vMonPos = { m_pBlackBoard->Get_Data().MonPos->m[3][0], m_pBlackBoard->Get_Data().MonPos->m[3][1], m_pBlackBoard->Get_Data().MonPos->m[3][2] };
	_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

	if (m_pBlackBoard->Get_Data().fAttack2Cool <= 0.f && m_isPage2 == false)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		return CNode::TREE_STATE::SUCCESS;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Attack2()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Attack2");

	if (m_pBlackBoard->Get_Data().IsAttack == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Condition_Attack1()
{
	if (m_pBlackBoard->Get_Data().IsChase == false)
		return CNode::TREE_STATE::FAILED;

	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_vector vMonPos = { m_pBlackBoard->Get_Data().MonPos->m[3][0], m_pBlackBoard->Get_Data().MonPos->m[3][1], m_pBlackBoard->Get_Data().MonPos->m[3][2] };
	_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

	if (fDis <= 3.f && m_isPage2 == false)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		return CNode::TREE_STATE::SUCCESS;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Attack1()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Attack1");

	if (m_pBlackBoard->Get_Data().IsAttack == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Condition_Walk()
{
	if (m_pBlackBoard->Get_Data().IsChase == true)
		return CNode::TREE_STATE::SUCCESS;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Walk()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Walk");

	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Idle()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Idle");

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
