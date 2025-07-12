#pragma once
#include "Item.h"

NS_BEGIN(Client)
class C2DItem abstract : public CItem
{
public:
	typedef struct tag2DItem_Desc : public CItem::ITEM_DESC
	{
		_float2		vMinUV;
		_float2		vMaxUV;
		_float2		vPos;

	}ITEM_2D_DESC;
protected:
	C2DItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	C2DItem(const C2DItem& Prototype);
	virtual ~C2DItem() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual HRESULT			Item_RenderSet(void* pArg);
protected:
	_float2					m_vLocalPos = {};
	_float2					m_vPos = {};

	_float2					m_vLocalSize = {};
	_float2					m_vSize = {};

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_float2					m_vMinUV = {}, m_vMaxUV = {};

protected:
	HRESULT					Bind_Transform_Resourc(class CShader* pShader);

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();
};
NS_END
