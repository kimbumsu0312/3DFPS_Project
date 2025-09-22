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
	CNode::TREE_STATE						Condition_Event_Check();

	CNode::TREE_STATE						Switch_Event_3();
	CNode::TREE_STATE						Switch_Event_2();
	CNode::TREE_STATE						Switch_Event_1();
	CNode::TREE_STATE						Switch_Spawn();

	CNode::TREE_STATE						Condition_Walk();
	CNode::TREE_STATE						Switch_Walk();

	CNode::TREE_STATE						Switch_Idle();

public:
	static CBehaviorTree_Bela* Create(CBlackBoard<CBela::BELA_DATA>* pData);
	virtual void							Free();
};
NS_END
