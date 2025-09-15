#pragma once
#include "WeaponObject.h"
#include "Monster_Normal.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CTexture;
NS_END

NS_BEGIN(Client)
class CNormon_Sword final : public CWeaponObject
{
public:
	typedef struct tagBodyNormalDesc : public CWeaponObject::WEAPON_DESC {
		CBlackBoard<CMonster_Normal::NORMALMON_DATA>* BlackBoard = { nullptr };
	}MONWEAPON_DESC;
private:
	CNormon_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormon_Sword(const CNormon_Sword& Prototype);
	virtual ~CNormon_Sword() = default;

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
	CTexture*				m_pNoiesTexCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

	CBlackBoard<CMonster_Normal::NORMALMON_DATA>* m_BlackBoard = { nullptr };
private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CNormon_Sword*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END