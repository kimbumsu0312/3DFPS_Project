#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CTrigger final : public CGameObject
{
public:

	typedef struct CTrigger_Desc{
		TRIGGER_TYPE			eType;
		OBJECT_TYPE				eObjType;
		_float3					vPos;

		_float3					vExtents;
		_float3					vCenter;
		function<void()>		TriggerEvent;
	}TRIGEER_DESC;

private:
	CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrigger(const CTrigger& Prototype);
	virtual ~CTrigger() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	virtual void			OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

private:
	CCollider*				m_pColliderCom = { nullptr };
	function<void()>		m_CTriggerEvent;
	OBJECT_TYPE				m_eObjType = {OBJECT_TYPE::END};
private:
	HRESULT					Ready_Componet(TRIGEER_DESC* pDesc);

public:
	static CTrigger*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();

};
NS_END	
