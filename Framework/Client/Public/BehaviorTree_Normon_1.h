#pragma once
#include "BehaviorTree.h"
#include "Monster_Normal.h"

NS_BEGIN(Client)
class CBehaviorTree_Normon_1 final : public CBehaviorTree
{
private:
	CBehaviorTree_Normon_1();
	virtual ~CBehaviorTree_Normon_1() = default;

public:
	virtual HRESULT							Initalize(CBlackBoard<CMonster_Normal::NORMALMON_DATA>* pData);
	virtual void							Update();

private:
	CBlackBoard<CMonster_Normal::NORMALMON_DATA>* m_pBlackBoard = { nullptr };

private:
	HRESULT									Ready_Node();

	CNode::TREE_STATE						Switch_Die();
	CNode::TREE_STATE						Switch_Damage();

	CNode::TREE_STATE						Condition_Attack();
	CNode::TREE_STATE						Switch_Attack();

	CNode::TREE_STATE						Condition_Chase();
	CNode::TREE_STATE						Switch_Chase();
	CNode::TREE_STATE						Switch_Idle();

public:
	static CBehaviorTree_Normon_1*			Create(CBlackBoard<CMonster_Normal::NORMALMON_DATA>* pData);
	virtual void							Free();
};
NS_END
