#pragma once
#include "UIObject.h"

NS_BEGIN(Client)
class CButton abstract : public CUIObject
{
protected:
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& Prototype);
	virtual ~CButton() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

protected:
	_bool					IsPick();
	_bool					IsClick_Up(MOUSEKEYSTATE iKey);
	_bool					IsClick_Down(MOUSEKEYSTATE iKey);
	_bool					IsClick_Holding(MOUSEKEYSTATE iKey);

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;

};
NS_END
