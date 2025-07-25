#pragma once
#include "ContainerObject.h"

NS_BEGIN(Engine)
NS_END

NS_BEGIN(Client)
class CPlayer final : public CContainerObject
{
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	_uint					m_iState = { };

private:
	HRESULT					Ready_Components();
	HRESULT					Ready_PartObjects();
public:
	static CPlayer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END