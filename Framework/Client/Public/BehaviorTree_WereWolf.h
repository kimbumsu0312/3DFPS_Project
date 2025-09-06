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

public:
	static CBehaviorTree_WereWolf*			Create(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData);
	virtual void							Free();
};
NS_END
