#pragma once
#include "BehaviorTree.h"
#include "Daniela.h"

NS_BEGIN(Client)
class CBehaviorTree_Daniela final : public CBehaviorTree
{
private:
	CBehaviorTree_Daniela();
	virtual ~CBehaviorTree_Daniela() = default;

public:
	virtual HRESULT							Initalize(CBlackBoard<CDaniela::DANIELA_DATA>* pData);
	virtual void							Update();

private:
	CBlackBoard<CDaniela::DANIELA_DATA>*	m_pBlackBoard = { nullptr };

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
	static CBehaviorTree_Daniela*			Create(CBlackBoard<CDaniela::DANIELA_DATA>* pData);
	virtual void							Free();
};
NS_END
