#pragma once
#include "Item.h"

NS_BEGIN(Client)
class C3DItem abstract : public CItem
{
protected:
	C3DItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	C3DItem(const C3DItem& Prototype);
	virtual ~C3DItem() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();
};

NS_END