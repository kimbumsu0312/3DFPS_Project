#include "pch.h"
#include "MonSpawner.h"
#include "SpawnPoint.h"

CMonSpawner::CMonSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject{pDevice, pContext}
{
}

CMonSpawner::CMonSpawner(const CMonSpawner& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CMonSpawner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonSpawner::Initialize(void* pArg)
{
	MONSPAWNERDESC* Desc = static_cast<MONSPAWNERDESC*>(pArg);
	m_MonDesc = Desc->MonDesc;
	m_szPoolPath = Desc->szPoolPath;
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(Desc->vSpawnerPostion, 1.f));
	return S_OK;
}

void CMonSpawner::Priority_Update(_float fTimeDelta)
{
}

void CMonSpawner::Update(_float fTimeDelta)
{
	if (m_IsSpawn)
		return;

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CMonSpawner::Late_Update(_float fTimeDelta)
{
	if (m_IsSpawn)
		return;

	if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom)))
		return;

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;
	__super::Late_Update(fTimeDelta);
#endif
}

HRESULT CMonSpawner::Render()
{
#ifdef _DEBUG
		m_pColliderCom->Render();
#endif
	return S_OK;
}

void CMonSpawner::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	switch (TargetCollision.iObjType)
	{
	case ENUM_CLASS(OBJECT_TYPE::PLAYER):
		for (_int i = 0; i < m_MonDesc.size(); ++i)
		{
			m_pGameInstance->Add_Pool_ToLayer(m_szPoolPath, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &m_MonDesc[i]);
		}
		m_IsSpawn = true;
		break;
	}
}

HRESULT CMonSpawner::Ready_Components(void* pArg)
{
	MONSPAWNERDESC* Desc = static_cast<MONSPAWNERDESC*>(pArg);
	CBounding_AABB::BOUNDING_AABB_DESC  AABBDesc{};
	AABBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::TRIGGER);
	AABBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::EVENT);
	AABBDesc.vExtents = Desc->vExtents;
	AABBDesc.vCenter = Desc->vCenter;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonSpawner::Ready_PartObjects()
{
	CSpawnPoint::SPAWNPOINTDESC Desc{};
	Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	for (_int i = 0; i < m_MonDesc.size(); ++i)
	{
		_wstring szPath = TEXT("Part_Point_%d") + i;
		Desc.vPostion = m_MonDesc[i].vPostion;
		if (FAILED(__super::Add_PartObject(szPath, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpawnPoint"), &Desc)))
			return E_FAIL;
	}
	return S_OK;
}

CMonSpawner* CMonSpawner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonSpawner* pInstance = new CMonSpawner(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonSpawner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonSpawner::Clone(void* pArg)
{
	CMonSpawner* pInstance = new CMonSpawner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CMonSpawner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonSpawner::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}
