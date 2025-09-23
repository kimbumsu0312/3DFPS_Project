#pragma once
#include "BehaviorTree.h"
#include "Alcina.h"

NS_BEGIN(Client)
class CBehaviorTree_Alcina final : public CBehaviorTree
{
private:
	CBehaviorTree_Alcina();
	virtual ~CBehaviorTree_Alcina() = default;

public:
	virtual HRESULT							Initalize(CBlackBoard<CAlcina::ALCHINA_DATA>* pData);
	virtual void							Update();

private:
	CBlackBoard<CAlcina::ALCHINA_DATA>*		m_pBlackBoard = { nullptr };

	_int									m_iMaxHp = {};
	_bool									m_isPage2 = {};
private:
	HRESULT									Ready_Node();
	
	CNode::TREE_STATE						Switch_Die();
	CNode::TREE_STATE						Switch_Damage();
	
	CNode::TREE_STATE						Condition_Event_Check();

	CNode::TREE_STATE						Switch_Event_1();

	CNode::TREE_STATE						Condition_Attack3();
	CNode::TREE_STATE						Switch_Attack3();
	CNode::TREE_STATE						Condition_Attack2();
	CNode::TREE_STATE						Switch_Attack2();
	CNode::TREE_STATE						Condition_Attack1();
	CNode::TREE_STATE						Switch_Attack1();

	CNode::TREE_STATE						Condition_Walk();
	CNode::TREE_STATE						Switch_Walk();
	CNode::TREE_STATE						Switch_Idle();

public:
	static CBehaviorTree_Alcina*			Create(CBlackBoard<CAlcina::ALCHINA_DATA>* pData);
	virtual void							Free();
};
NS_END
