#include "pch.h"
#include "Trigger.h"

CTrigger::CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject{pDevice, pContext}
{
}

CTrigger::CTrigger(const CTrigger& Prototype) : CGameObject(Prototype)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
	TRIGEER_DESC* pDesc = static_cast<TRIGEER_DESC*>(pArg);
	m_eObjType = pDesc->eObjType;
	m_CTriggerEvent = pDesc->TriggerEvent;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Ready_Componet(pDesc);

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vPos), 1.f));

	return S_OK;
}

void CTrigger::Priority_Update(_float fTimeDelta)
{
}

void CTrigger::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTrigger::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom)))
		return;
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

}

HRESULT CTrigger::Render()
{
	m_pColliderCom->Render();
	return S_OK;
}

void CTrigger::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	if (ENUM_CLASS(m_eObjType) == TargetCollision.iObjType)
		m_CTriggerEvent();
}

HRESULT CTrigger::Ready_Componet(TRIGEER_DESC* pDesc)
{
	CBounding_AABB::BOUNDING_AABB_DESC  AABBDesc{};
	switch (pDesc->eType)
	{
	case TRIGGER_TYPE::PLAYER:
		AABBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::TRIGGER_PLAYER);
		break;
	case TRIGGER_TYPE::MONSTER:
		AABBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::TRIGGER_MON);
		break;
	}
	
	AABBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::END);
	AABBDesc.vExtents = pDesc->vExtents;
	AABBDesc.vCenter = pDesc->vCenter;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

CTrigger* CTrigger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrigger* pInstance = new CTrigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTrigger"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrigger::Clone(void* pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTrigger"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();
}
