#pragma once
#include "BehaviorTree.h"
#include "Daniela.h"

NS_BEGIN(Client)
class CBehaviorTree_Bela final : public CBehaviorTree
{
private:
	CBehaviorTree_Bela();
	virtual ~CBehaviorTree_Bela() = default;

public:
	virtual HRESULT							Initalize(CBlackBoard<CBela::BELA_DATA>* pData);
	virtual void							Update();

private:
	CBlackBoard<CBela::BELA_DATA>* m_pBlackBoard = { nullptr };

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
	static CBehaviorTree_Bela* Create(CBlackBoard<CBela::BELA_DATA>* pData);
	virtual void							Free();
};
NS_END
