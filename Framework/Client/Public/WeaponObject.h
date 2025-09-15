#pragma once
#include "PartObject.h"

NS_BEGIN(Client)
class CWeaponObject abstract : public CPartObject
{
public:
	typedef struct tagWeaponDesc : public CPartObject::PARTOBJECT_DESC
	{
		_wstring*				pCulStateTag = {};
		const _float4x4*		pSocketMatrix = { nullptr };
	}WEAPON_DESC;

protected:
	CWeaponObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeaponObject(const CWeaponObject& Prototype);
	virtual ~CWeaponObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	virtual HRESULT			Add_Collider();

	
protected:
	_wstring*   			m_pCulStateTag = {};
	const _float4x4*		m_pSocketMatrix = { nullptr };
public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};

NS_END