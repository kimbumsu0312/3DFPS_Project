#include "pch.h"
#include "Bela.h"
#include "Player_Manager.h"

#include "Body_Bela.h"
#include "WeaponObject.h"
#include "BehaviorTree_Bela.h"

#include "Idle_Bela.h"
#include "Die_Bela.h"
#include "Walk_Bela.h"
#include "Spawn_Bela.h"
#include "Event_1_Bela.h"
#include "Event_2_Bela.h"
#include "Event_3_Bela.h"

CBela::CBela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{
}

CBela::CBela(const CBela& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CBela::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBela::Initialize(void* pArg)
{
	m_iAnimState = ENUM_CLASS(ANIM_STATE::NORMAL);
	m_szAnimTag = "Idle_Freezes";

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Utility()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;

	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::Body)] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::Head)] = m_pBodyObject->Get_BoneMatrix(TEXT("Head"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::L_ARM)] = m_pBodyObject->Get_BoneMatrix(TEXT("L_UpperArm"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::R_ARM)] = m_pBodyObject->Get_BoneMatrix(TEXT("R_UpperArm"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-61.79f, -11.11f, 46.83f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(-90.f));

	m_pGameInstance->Subscribe<Event_BelaSpawn>([&](const Event_BelaSpawn& e) {Event_Spawn(); });

	return S_OK;
}

void CBela::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(DIK_K))
		m_BlackBoard->Set_Data().IsSpawn == true ? m_BlackBoard->Set_Data().IsSpawn = false : m_BlackBoard->Set_Data().IsSpawn = true;

	if (m_pGameInstance->IsKeyDown(DIK_L))
		m_BlackBoard->Get_Data().iHp <= 0 ? m_BlackBoard->Set_Data().iHp = 100 : m_BlackBoard->Set_Data().iHp = 0;

	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);
	m_pWeaponObject->Priority_Update(fTimeDelta);

	if (m_BlackBoard->Get_Data().bIsFly == true)
		m_pEffect->Priority_Update(fTimeDelta);
	if (m_BlackBoard->Get_Data().bIsSpawnFly == true)
		m_pSpawnEffect->Priority_Update(fTimeDelta);
}

void CBela::Update(_float fTimeDelta)
{
	m_pBehaviorTree->Update();
	State_Change();
	m_pCulStateObject->Update(this, fTimeDelta);

	Root_Move();

	m_pBodyObject->Update(fTimeDelta);
	m_pWeaponObject->Update(fTimeDelta);

	Collider_Update();
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 1.5f);
	
	if (m_BlackBoard->Get_Data().bIsFly == true)
	{
		m_pEffect->Set_Potion(vPos);
		m_pEffect->Update(fTimeDelta);
	}
	if (m_BlackBoard->Get_Data().bIsSpawnFly == true)
	{
		m_pSpawnEffect->Set_Potion(vPos);
		m_pSpawnEffect->Update(fTimeDelta);
	}

}

void CBela::Late_Update(_float fTimeDelta)
{
	if (m_szCulStateTag == TEXT("Idle"))
		return;

	if (m_BlackBoard->Get_Data().fNoies < 0.8f)
	{
		for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
		{

			if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
				return;
		}
	}
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);
	m_pWeaponObject->Late_Update(fTimeDelta, m_BlackBoard->Get_Data().fNoies );
	
	if (m_BlackBoard->Get_Data().bIsFly == true)
		m_pEffect->Late_Update(fTimeDelta);

	if (m_BlackBoard->Get_Data().bIsSpawnFly == true)
		m_pSpawnEffect->Late_Update(fTimeDelta);
}

HRESULT CBela::Render()
{
#ifdef _DEBUG
	for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
	{
		m_pColliderCom[i]->Render();
	}

#endif
	return S_OK;
}

void CBela::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

void CBela::Target_LookTurn(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();

	_vector vDir = XMVector3Normalize(vPlayerPos - vMonPos);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CBela::Target_LookAt()
{
	m_pTransformCom->LookAt(CPlayer_Manager::GetInstance()->Get_PlayerPos());
}

void CBela::Attack_Collision()
{
	if (FAILED(m_pWeaponObject->Add_Collider()))
			return;
}

void CBela::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
	{
		if (TargetCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
			m_pTransformCom->Is_Sliding(m_pNavigationCom, XMLoadFloat3(&TargetCollision.pCollider->Get_Intersect_Normal()));
	}
	else
	{
		CBlood_Effect::BLODE_EFFECT_INIT Desc;

		switch (TargetCollision.iObjType)
		{
		case ENUM_CLASS(OBJECT_TYPE::RAY):
			if (m_BlackBoard->Get_Data().IsFreezes == true)
			{
				m_BlackBoard->Set_Data().IsDamage = true;
				Desc.vPos = TargetCollision.RayDesc.OnCloiderPos;
				m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Blood"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &Desc);
			}
			break;
		}
	}
}

void CBela::Spawn_EffectReset()
{
	m_pSpawnEffect->Reset();
}

void CBela::SetUp_Node()
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vNextPos{};
	_vector vPlayerPos{};

	XMStoreFloat3(&vNextPos, CPlayer_Manager::GetInstance()->Get_PlayerPos());
	vPlayerPos = XMVectorSetW(XMLoadFloat3(&vNextPos), 1.f);
	m_pNavigationCom->SetUp_Node(CPlayer_Manager::GetInstance()->Get_CellIndex(), vNextPos);
}

void CBela::Target_LookTurn_Navi(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vNextPos{};
	XMStoreFloat3(&vNextPos, CPlayer_Manager::GetInstance()->Get_PlayerPos());

	if (m_pNavigationCom->IsNaviNode(vMonPos, vNextPos) == true)
		SetUp_Node();

	_vector vDir = XMVector3Normalize(XMVectorSetY(XMLoadFloat3(&vNextPos) - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK), 0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta * 5.f);
}

HRESULT CBela::Ready_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.2f, 0.67f, 0.2f);
	OBBDesc.vCenter = _float3(0.f, -0.15f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::Body)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_HEAD);
	OBBDesc.vExtents = _float3(0.12f, 0.15f, 0.12f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::Head)]), &OBBDesc)))
		return E_FAIL;
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(0.1f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_ARM_L"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::L_ARM)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(-0.1f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_ARM_R"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::R_ARM)]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 2815;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CBela::Ready_PartObjects()
{
	CBody_Bela::BODY_DESC BodyDesc{};
	BodyDesc.pBlackBoard = m_BlackBoard;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();


	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Bela"), &BodyDesc)))
		return E_FAIL;

	CBody_Bela* pBody = static_cast<CBody_Bela*>(Find_PartObject(TEXT("Part_Body")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pBodyObject = pBody;
	Safe_AddRef(m_pBodyObject);

	CWeaponObject::WEAPON_DESC WeaponDesc{};
	WeaponDesc.pCulStateTag = &m_szCulStateTag;
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_Wep"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_Shotel"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Boss_Shotel"), &WeaponDesc)))
		return E_FAIL;

	CBoss_Shotel* pWeaponObject = static_cast<CBoss_Shotel*>(Find_PartObject(TEXT("Part_Shotel")));

	if (pWeaponObject == nullptr)
		return E_FAIL;

	m_pWeaponObject = pWeaponObject;
	Safe_AddRef(m_pWeaponObject);

	CFly_Effect::FLY_EFFECT_DESC FlyDesc{};
	FlyDesc.isDead = false;
	FlyDesc.eType = CFly_Effect::Fly_Type::SPIN;
	FlyDesc.szPoolingPath = TEXT("Pool_Fly_Spin");
	FlyDesc.fRotationPerSec = 1.f;
	FlyDesc.fSpeedPerSec = 1.f;

	m_pEffect = dynamic_cast<CFly_Effect*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fly_Effect"), &FlyDesc));
	
	FlyDesc.eType = CFly_Effect::Fly_Type::SPREAD;
	FlyDesc.szPoolingPath = TEXT("Pool_Fly_Spread");
	FlyDesc.fRotationPerSec = 1.f;
	FlyDesc.fSpeedPerSec = 1.f;
	
	m_pSpawnEffect = dynamic_cast<CFly_Effect*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fly_Effect"), &FlyDesc));
	return S_OK;
}

HRESULT CBela::Ready_Utility()
{
	m_BlackBoard = CBlackBoard<BELA_DATA>::Create();
	m_BlackBoard->Set_Data().iAnimState = &m_iAnimState;
	m_BlackBoard->Set_Data().szAnimTag = &m_szAnimTag;
	m_BlackBoard->Set_Data().bIsAnimFinsh = &m_bIsAnimFinsh;
	m_BlackBoard->Set_Data().bIsAnimLoop = &m_bIsAnimLoop;
	m_BlackBoard->Set_Data().szCulStateTag = &m_szCulStateTag;

	m_BlackBoard->Set_Data().iHp = 1000;

	m_BlackBoard->Set_Data().IsDamage = false;
	m_BlackBoard->Set_Data().IsHitPoint.IsBody = false;
	m_BlackBoard->Set_Data().IsHitPoint.IsHead = false;
	m_BlackBoard->Set_Data().IsHitPoint.IsSholder_L = false;
	m_BlackBoard->Set_Data().IsHitPoint.isSholder_R = false;

	m_BlackBoard->Set_Data().IsChase = false;

	m_BlackBoard->Set_Data().MonPos = m_pTransformCom->Get_WorldMatrixPtr();
	m_BlackBoard->Set_Data().fNoies = 0.f;
	m_BlackBoard->Set_Data().fFreezes = 0.f;
	m_BlackBoard->Set_Data().bIsFly = false;
	m_BlackBoard->Set_Data().bIsSpawnFly = false;
	m_BlackBoard->Set_Data().fMoveSpeed = 3.f;
	m_BlackBoard->Set_Data().IsFreezes = false;

	m_pBehaviorTree = CBehaviorTree_Bela::Create(m_BlackBoard);

	return S_OK;
}

HRESULT CBela::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_Bela::Create());
	Add_StateObject(TEXT("Walk"), CWalk_Bela::Create());
	Add_StateObject(TEXT("Die"), CDie_Bela::Create());

	Add_StateObject(TEXT("Spawn"), CSpawn_Bela::Create());
	Add_StateObject(TEXT("Event_1"), CEvent_1_Bela::Create());
	Add_StateObject(TEXT("Event_2"), CEvent_2_Bela::Create());
	Add_StateObject(TEXT("Event_3"), CEvent_3_Bela::Create());

	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
	return S_OK;
}

HRESULT CBela::Ready_TriggerEvent()
{
	CTrigger::TRIGEER_DESC TriggerDesc;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_2(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 4.f, 4.f, 4.f };
	TriggerDesc.vPos = _float3{ -69.40f, -8.68f, 30.08f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_3(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 4.f, 4.f, 4.f };
	TriggerDesc.vPos = _float3{ -56.29f, -8.68f, 29.79f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_DIe(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 1.f, 1.f, 1.f };
	TriggerDesc.vPos = _float3{ -33.32f, -8.69f, 27.04f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CBela::Add_StateObject(const _wstring& strStateObjectTag, CMonState_Bela* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CMonState_Bela* CBela::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CBela::State_Change()
{
	if (m_szPreStateTag != m_szCulStateTag)
	{
		m_pCulStateObject->Exit(this);
		Safe_Release(m_pCulStateObject);

		m_pCulStateObject = Find_StateObject(m_szCulStateTag);
		Safe_AddRef(m_pCulStateObject);

		m_pCulStateObject->Enter(this);
		m_szPreStateTag = m_szCulStateTag;
	}
}

void CBela::Root_Move()
{
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);

	//월드 분해
	_vector vScale, vWorldRot, vWorldTrans;
	XMMatrixDecompose(&vScale, &vWorldRot, &vWorldTrans, m_pTransformCom->Get_WorldMatrix());
	_vector vMovePos = XMLoadFloat3(m_pBodyObject->Get_MovePos());
	_vector vMoveRot = XMLoadFloat4(m_pBodyObject->Get_MoveRot());

	//월드 기준으로 방향 보정
	vMovePos = XMVector3Rotate(vMovePos, vWorldRot);

	//이동량 누적
	vWorldTrans += vMovePos;
	//회전량 누적
	vWorldRot = XMQuaternionMultiply(XMVectorSetW(vMoveRot, 1.f), vWorldRot);
	vWorldRot = XMQuaternionNormalize(vWorldRot);

	_matrix ScaleMat = XMMatrixScalingFromVector(vScale);
	_matrix Movemat = XMMatrixTranslationFromVector(vWorldTrans);
	_matrix RotMat = XMMatrixRotationQuaternion(vWorldRot);

	_float4x4 WorldMatrix = {};
	XMStoreFloat4x4(&WorldMatrix, XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vWorldRot, vWorldTrans));

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	if (false == m_pNavigationCom->isMove(m_pTransformCom->Get_State(STATE::POSITION)))
		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(vPos, 1.f));

	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));

}

void CBela::Collider_Update()
{
	_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
	_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

	for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
	{
		_matrix BoneMat = XMLoadFloat4x4(m_pColliderBone[i]);
		_vector vScale, vRot, vTrans;
		XMMatrixDecompose(&vScale, &vRot, &vTrans, BoneMat);

		_matrix WorldRotMat = XMMatrixRotationQuaternion(vRot);
		_matrix WorldTransMat = XMMatrixTranslationFromVector(vTrans);
		_matrix WorldMatrix = WorldRotMat * WorldTransMat * Worldmat;

		m_pColliderCom[i]->Update(WorldMatrix);
	}
}

void CBela::Event_Spawn()
{
	m_BlackBoard->Set_Data().IsSpawn = true;
	Ready_TriggerEvent();
}

void CBela::Event_2()
{
	m_BlackBoard->Set_Data().IsEvent_2 = true;
}

void CBela::Event_3()
{
	m_BlackBoard->Set_Data().IsEvent_3 = true;
}

void CBela::Event_DIe()
{
	m_BlackBoard->Set_Data().iHp = 0;
}

CBela* CBela::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBela* pInstance = new CBela(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CBela"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBela::Clone(void* pArg)
{
	CBela* pInstance = new CBela(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBela"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBela::Free()
{
	__super::Free();
	for (auto& Pair : m_StateObjects)
		Safe_Release(Pair.second);

	m_StateObjects.clear();

	Safe_Release(m_pCulStateObject);
	Safe_Release(m_pWeaponObject);
	Safe_Release(m_pBodyObject);
	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_BlackBoard);
	Safe_Release(m_pBehaviorTree);
	Safe_Release(m_pEffect);
	Safe_Release(m_pSpawnEffect);
} 
