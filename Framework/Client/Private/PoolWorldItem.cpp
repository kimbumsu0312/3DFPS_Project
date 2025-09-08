#include "pch.h"
#include "PoolWorldItem.h"
#include "Player_Manager.h"
#include "Inven_Manager.h"
#include "Get_UI.h"

CPoolWorld_Item::CPoolWorld_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingObject(pDevice, pContext)
{
}

CPoolWorld_Item::CPoolWorld_Item(const CPoolWorld_Item& Prototype) : CPoolingObject(Prototype)
{
}

HRESULT CPoolWorld_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPoolWorld_Item::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* Desc = static_cast<GAMEOBJECT_DESC*>(pArg);

	for (auto ItemData : g_3DItemData)
	{
		if (ItemData.m_szModelPath == Desc->szModel_Path)
		{
			m_iItemIndex = ItemData.m_iItemID;
			break;
		}
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(Desc->szModel_Path)))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_pTransformCom->Scale(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

void CPoolWorld_Item::Priority_Update(_float fTimeDelta)
{
	m_pGuideUI->Priority_Update(fTimeDelta);
}

void CPoolWorld_Item::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));

	for (auto Collider : m_pColliderCom)
	{
		Collider->Update(m_pTransformCom->Get_WorldMatrix());
	}
}

void CPoolWorld_Item::Late_Update(_float fTimeDelta)
{
	for (auto Collider : m_pColliderCom)
	{
		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, Collider)))
			return;
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

#ifdef _DEBUG
	for (auto Collider : m_pColliderCom)
		m_pGameInstance->Add_DebugComponent(Collider);
#endif
}

HRESULT CPoolWorld_Item::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
			continue;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CPoolWorld_Item::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	if(MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::ITEM))
	{ 
		switch (TargetCollision.iObjType)
		{
		case ENUM_CLASS(OBJECT_TYPE::PLAYER_VIEW):
			m_pGuideUI->IsOn();
			if (m_pGameInstance->IsKeyDown(DIK_F))
			{
				if (CInven_Manager::GetInstance()->Add_ItemSlot(m_iItemIndex, m_szPoolPath))
				{
					SetDead();
				}
			}
			break;
		}
	}
	else if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::ITEM_DETACT))
	{
		switch (TargetCollision.iObjType)
		{
		case ENUM_CLASS(OBJECT_TYPE::PLAYER):
			m_pGuideUI->Update_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
			break;
		}
	}
	

}

HRESULT CPoolWorld_Item::Initialize_Pool(void* pArg)
{
	POOLITEM_DESC* pDesc = static_cast<POOLITEM_DESC*>(pArg);
	m_szPoolPath = pDesc->szPoolPath;
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPos, 1.f));
	m_pNavigationCom->Set_CellIndex(pDesc->vNaviIndex);
	return S_OK;
}

void CPoolWorld_Item::Return_Pool()
{
	m_bIsDead = false;
}

HRESULT CPoolWorld_Item::Ready_Components(_wstring szModelPath)
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::ITEM);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ITEM_DETACT);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(2.5f, 2.5f, 2.5f);
	OBBDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_DETACT"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ITEM_COLLIDER::DETACT_REAGE)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::ITEM);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ITEM);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.2f, 0.2f, 0.2f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ITEM_COLLIDER::ITEM)]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = -1;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szModelPath,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPoolWorld_Item::Ready_UI()
{
	CGet_UI::GET_UI_DESC Desc{};

	Desc.eFontType = CGet_UI::Font_Type::GET;

	m_pGuideUI = static_cast<CGet_UI*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_GetUI"), &Desc));
	return S_OK;
}

HRESULT CPoolWorld_Item::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;


	return S_OK;
}

CPoolWorld_Item* CPoolWorld_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPoolWorld_Item* pInstance = new CPoolWorld_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CPoolWorld_Item"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPoolWorld_Item::Clone(void* pArg)
{
	CPoolWorld_Item* pInstance = new CPoolWorld_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPoolWorld_Item"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPoolWorld_Item::Free()
{
	__super::Free();
	for (auto Collider : m_pColliderCom)
	{
		Safe_Release(Collider);
	}
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pGuideUI);
}
