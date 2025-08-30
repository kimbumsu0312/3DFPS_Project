#pragma once
#include "Base.h"

NS_BEGIN(Client)
template<typename T>
class CBlackBoard final : public CBase
{
private:
	CBlackBoard() {};
	virtual ~CBlackBoard() = default;

public:
	 T&						Set_Data()  { return m_Data; }
	 const T&				Get_Data() const { return m_Data; }

private:
	T						m_Data;
public:
	static	CBlackBoard<T>* Create() {return new CBlackBoard<T>();}
	void					Free()  { __super::Free(); }
};

NS_END

