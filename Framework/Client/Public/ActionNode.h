#pragma once
#include "Node.h"

NS_BEGIN(Client)
class CActionNode final : public CNode
{
private:
	CActionNode();
	virtual ~CActionNode() = default;

public:
	HRESULT				Init_Action(function <TREE_STATE()> Action);
	virtual TREE_STATE	Evaluat() override;
	
private:
	function<TREE_STATE()>	m_Action;

public:
	static CActionNode*	Create(function<TREE_STATE()> Action);
	virtual void	Free() override;

};

NS_END