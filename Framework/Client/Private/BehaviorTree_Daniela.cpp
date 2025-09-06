#include "pch.h"
#include "BehaviorTree_Daniela.h"
#include "Daniela.h"
#include "Player_Manager.h"

CBehaviorTree_Daniela::CBehaviorTree_Daniela()
{
}

HRESULT CBehaviorTree_Daniela::Initalize(CBlackBoard<CDaniela::DANIELA_DATA>* pData)
{
	if (pData == nullptr)
		return E_FAIL;
	m_pBlackBoard = pData;
	Safe_AddRef(m_pBlackBoard);

	if (FAILED(Ready_Node()))
		return E_FAIL;

	return S_OK;
}

void CBehaviorTree_Daniela::Update()
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

HRESULT CBehaviorTree_Daniela::Ready_Node()
{
	CSeletctorNode* pRoot = CSeletctorNode::Create();

	CSequenceNode* pAttackSequence = CSequenceNode::Create();
	CActionNode* pAttackCondition = CActionNode::Create([&]() {return Condition_Attack(); });
	CActionNode* pAttackSwitch = CActionNode::Create([&]() {return Switch_Attack(); });
	pAttackSequence->Add_Node(pAttackCondition);
	pAttackSequence->Add_Node(pAttackSwitch);

	CSequenceNode* pWalkSequence = CSequenceNode::Create();
	CActionNode* pWalkCondition = CActionNode::Create([&]() {return Condition_Chase(); });
	CActionNode* pWalkSwitch = CActionNode::Create([&]() {return Switch_Chase(); });
	pWalkSequence->Add_Node(pWalkCondition);
	pWalkSequence->Add_Node(pWalkSwitch);

	CActionNode* pIdleSwitch = CActionNode::Create([&]() {return Switch_Idle(); });

	pRoot->Add_Node(pAttackSequence);
	pRoot->Add_Node(pWalkSequence);
	pRoot->Add_Node(pIdleSwitch);

	m_pRootNode = pRoot;
	return S_OK;
}

CNode::TREE_STATE CBehaviorTree_Daniela::Switch_Die()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Die");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Daniela::Switch_Damage()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Damage");
	m_pRootNode->Reset();
	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Daniela::Condition_Attack()
{
	if (m_pBlackBoard->Get_Data().IsChase == false || m_pBlackBoard->Get_Data().IsIdle == true)
		return CNode::TREE_STATE::FAILED;

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

CNode::TREE_STATE CBehaviorTree_Daniela::Switch_Attack()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Attack");

	if (m_pBlackBoard->Get_Data().IsAttack == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Daniela::Condition_Chase()
{
	if (m_pBlackBoard->Get_Data().IsChase == true && m_pBlackBoard->Get_Data().IsIdle == false)
		return CNode::TREE_STATE::SUCCESS;

	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Daniela::Switch_Chase()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Chase");

	return CNode::TREE_STATE::SUCCESS;
}

CNode::TREE_STATE CBehaviorTree_Daniela::Switch_Idle()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Idle");

	return CNode::TREE_STATE::SUCCESS;
}

CBehaviorTree_Daniela* CBehaviorTree_Daniela::Create(CBlackBoard<CDaniela::DANIELA_DATA>* pData)
{
	CBehaviorTree_Daniela* pInstance = new CBehaviorTree_Daniela();

	if (FAILED(pInstance->Initalize(pData)))
	{
		MSG_BOX(TEXT("Failed to Create : CBehaviorTree_Daniela"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBehaviorTree_Daniela::Free()
{
	__super::Free();

	Safe_Release(m_pBlackBoard);
}
