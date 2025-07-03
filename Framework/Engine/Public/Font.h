#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CFont final : public CBase
{
private:
	CFont();
	virtual ~CFont() = default;
	
public:
	virtual void	Initialize();

private:
	//SpriteFont		m_Font;		//�ε��� ��Ʈ ����
	//_float2			m_vPos = {};
	//_float4			m_vColor = {};

public:
	static CFont*	Create();
	virtual void	Free();
};
NS_END
