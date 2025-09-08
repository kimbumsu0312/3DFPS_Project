#pragma once
#include "BehaviorTree.h"
#include "Monster_WereWolf.h"

NS_BEGIN(Client)
class CBehaviorTree_WereWolf final : public CBehaviorTree
{
private:
	CBehaviorTree_WereWolf();
	virtual ~CBehaviorTree_WereWolf() = default;

public:
	virtual HRESULT							Initalize(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData);
	virtual void							Update();

private:
	CBlackBoard<
		CMonster_WereWolf::WereWolfData>*	m_pBlackBoard = { nullptr };

private:
	HRESULT									Ready_Node();

	CNode::TREE_STATE						Condition_Event_Check();
	CNode::TREE_STATE						Condition_Event_3();
	CNode::TREE_STATE						Switch_Event_3();

	CNode::TREE_STATE						Condition_Event_2();
	CNode::TREE_STATE						Switch_Event_2();

	CNode::TREE_STATE						Condition_Event_1();
	CNode::TREE_STATE						Switch_Event_1();

	CNode::TREE_STATE						Condition_Attack();
	CNode::TREE_STATE						Switch_Attack();

	CNode::TREE_STATE						Switch_Walk();

	CNode::TREE_STATE						Condition_Idle();
	CNode::TREE_STATE						Switch_Idle();

public:
	static CBehaviorTree_WereWolf*			Create(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData);
	virtual void							Free();
};
NS_END
