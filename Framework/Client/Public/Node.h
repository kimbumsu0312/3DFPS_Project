#pragma once
#include "Base.h"

NS_BEGIN(Client)
class CNode abstract : public CBase
{
public:
	enum class TREE_STATE { RUN, SUCCESS, FAILED, END };

protected:
	CNode();
	virtual ~CNode() = default;

public:
	virtual TREE_STATE	Evaluat() = 0 ;
	virtual void		Reset();

protected:
	_int				m_iTreeIndex = {};

public:
	virtual void		Free() override;

};

NS_END