#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CItem abstract : public CGameObject
{
public:
	typedef struct tagItem_Desc
	{
	}ITEM_DESC;

protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& Prototype);
	virtual ~CItem() = default;

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
	_uint					m_iItemIndex = {};

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free();
};

NS_END