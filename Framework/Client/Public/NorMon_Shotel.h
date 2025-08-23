#pragma once
#include "WeaponObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)
class CNormon_Shotel final : public CWeaponObject
{
private:
	CNormon_Shotel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormon_Shotel(const CNormon_Shotel& Prototype);
	virtual ~CNormon_Shotel() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	virtual HRESULT			Add_Collider() override;
	virtual void			OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

private:
	CCollider*				m_pColliderCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CNormon_Shotel*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END