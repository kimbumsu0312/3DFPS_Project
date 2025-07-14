#pragma once
#include "2DItem.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer;
NS_END

NS_BEGIN(Client)
class Item_2D_Render final : public C2DItem
{
protected:
	Item_2D_Render(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Item_2D_Render(const Item_2D_Render& Prototype);
	virtual ~Item_2D_Render() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	static Item_2D_Render*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END