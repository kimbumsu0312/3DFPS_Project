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

	__super::Update();
}

HRESULT CBehaviorTree_Alcina::Ready_Node()
{
	CSeletctorNode* pRoot = CSeletctorNode::Create();

	CSequenceNode* pAttackSequence = CSequenceNode::Create();
	CActionNode* pAttackCondition = CActionNode::Create([&]() {return Condition_Attack(); });
	CActionNode* pAttackSwitch = CActionNode::Create([&]() {return Switch_Attack(); });
	pAttackSequence->Add_Node(pAttackCondition);
	pAttackSequence->Add_Node(pAttackSwitch);

	CSequenceNode* pWalkSequence = CSequenceNode::Create();
	CActionNode* pWalkCondition = CActionNode::Create([&]() {return Condition_Walk(); });
	CActionNode* pWalkSwitch = CActionNode::Create([&]() {return Switch_Walk(); });
	pWalkSequence->Add_Node(pWalkCondition);
	pWalkSequence->Add_Node(pWalkSwitch);

	CActionNode* pIdleSwitch = CActionNode::Create([&]() {return Switch_Idle(); });

	pRoot->Add_Node(pAttackSequence);
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

CNode::TREE_STATE CBehaviorTree_Alcina::Condition_Attack()
{
	if (m_pBlackBoard->Get_Data().IsChase == false)
		return CNode::TREE_STATE::FAILED;

	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_vector vMonPos = { m_pBlackBoard->Get_Data().MonPos->m[3][0], m_pBlackBoard->Get_Data().MonPos->m[3][1], m_pBlackBoard->Get_Data().MonPos->m[3][2] };
	_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

	if (fDis >= 6.f && m_pBlackBoard->Get_Data().fAttack1Cool <= 0.f)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		m_pBlackBoard->Set_Data().eAttackType = CAlcina::Attack_Type::LONG;
		return CNode::TREE_STATE::SUCCESS;
	}
	else if (fDis >= 4.f && m_pBlackBoard->Get_Data().fAttack2Cool <= 0.f)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		m_pBlackBoard->Set_Data().eAttackType = CAlcina::Attack_Type::SHORT;
		return CNode::TREE_STATE::SUCCESS;
	}
	else if (fDis >= 2.f && m_pBlackBoard->Get_Data().fAttack3Cool <= 0.f)
	{
		m_pBlackBoard->Set_Data().IsAttack = true;
		m_pBlackBoard->Set_Data().eAttackType = CAlcina::Attack_Type::ZERO;
		return CNode::TREE_STATE::SUCCESS;
	}
	return CNode::TREE_STATE::FAILED;
}

CNode::TREE_STATE CBehaviorTree_Alcina::Switch_Attack()
{
	*m_pBlackBoard->Set_Data().szCulStateTag = TEXT("Attack");
	
	if(m_pBlackBoard->Get_Data().IsAttack == true)
		return CNode::TREE_STATE::RUN;

	return CNode::TREE_STATE::SUCCESS;
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
