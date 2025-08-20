#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CMapNevi final : public CGameObject
{
private:
	CMapNevi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapNevi(const CMapNevi& Prototype);
	virtual ~CMapNevi() = default;
public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CNavigation*			m_pNavigationCom = { nullptr };
	HRESULT					Ready_Components();

public:
	static CMapNevi*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END