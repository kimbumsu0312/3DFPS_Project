#include "pch.h"
#include "Daniela.h"
#include "Player_Manager.h"

#include "Body_Daniela.h"
#include "WeaponObject.h"
#include "BehaviorTree_Daniela.h"

#include "Idle_Daniela.h"
#include "Die_Daniela.h"
#include "Damage_Daniela.h"
#include "Chase_Daniela.h"
#include "Attack_Daniela.h"
#include "Critical_Attack_Daniela.h"

CDaniela::CDaniela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{
}

CDaniela::CDaniela(const CDaniela& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CDaniela::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDaniela::Initialize(void* pArg)
{
	m_iAnimState = ENUM_CLASS(ANIM_STATE::NORMAL);
	m_szAnimTag = "Idle";

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Utility()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;

	if (FAILED(Ready_TriggerEvent()))
		return E_FAIL;
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::Body)] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::Head)] = m_pBodyObject->Get_BoneMatrix(TEXT("Head"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::L_ARM)] = m_pBodyObject->Get_BoneMatrix(TEXT("L_UpperArm"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::R_ARM)] = m_pBodyObject->Get_BoneMatrix(TEXT("R_UpperArm"));


	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-35.49f, -3.44f, 46.34f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(110.f));
	return S_OK;
}

void CDaniela::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsStart)
		return;

	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);
	m_pWeaponObject->Priority_Update(fTimeDelta);
}

void CDaniela::Update(_float fTimeDelta)
{
	if (m_BlackBoard->Get_Data().fAttackCool > 0.f)
		m_BlackBoard->Set_Data().fAttackCool -= fTimeDelta;

	if (m_BlackBoard->Get_Data().fCriAttackCool > 0.f)
		m_BlackBoard->Set_Data().fCriAttackCool -= fTimeDelta;

	if(m_BlackBoard->Get_Data().fDamage_Cool > 0.f)
		m_BlackBoard->Set_Data().fDamage_Cool -= fTimeDelta;

	m_pBehaviorTree->Update();

	State_Change();
	m_pCulStateObject->Update(this, fTimeDelta);

	Root_Move();
	
	m_pBodyObject->Update(fTimeDelta);
	m_pWeaponObject->Update(fTimeDelta);

	if(m_bIsStart)
		Collider_Update();
}

void CDaniela::Late_Update(_float fTimeDelta)
{
	if (m_bIsStart)
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
	m_pWeaponObject->Late_Update(fTimeDelta, m_BlackBoard->Get_Data().fNoies);
	
}

HRESULT CDaniela::Render()
{
#ifdef _DEBUG
	for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
	{
		m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
	}
#endif
	return S_OK;
}

void CDaniela::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

void CDaniela::Target_LookTurn(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();

	_vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK),0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));
	 
	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CDaniela::Target_LookTurn_Navi(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vNextPos{};
	XMStoreFloat3(&vNextPos, CPlayer_Manager::GetInstance()->Get_PlayerPos());
	
	if (m_pNavigationCom->IsNaviNode(vMonPos, vNextPos) == true)
		SetUp_Node();

	_vector vDir = XMVector3Normalize(XMVectorSetY(XMLoadFloat3(&vNextPos) - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK), 0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta*3.f);

}

void CDaniela::SetUp_Node()
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vNextPos{};
	_vector vPlayerPos{};

	XMStoreFloat3(&vNextPos, CPlayer_Manager::GetInstance()->Get_PlayerPos());
	vPlayerPos = XMVectorSetW(XMLoadFloat3(&vNextPos), 1.f);
	m_pNavigationCom->SetUp_Node(CPlayer_Manager::GetInstance()->Get_CellIndex(), vNextPos);
}

void CDaniela::Attack_Collision()
{
	if (FAILED(m_pWeaponObject->Add_Collider()))
		return;
}

void CDaniela::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
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

			if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::MON_HEAD))
				m_BlackBoard->Set_Data().IsHitPoint.IsHead = true;
			if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::MON_BODY))
				m_BlackBoard->Set_Data().IsHitPoint.IsBody = true;
			if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::MON_SHOULDER_R))
				m_BlackBoard->Set_Data().IsHitPoint.isSholder_R = true;
			if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::MON_SHOULDER_L))
				m_BlackBoard->Set_Data().IsHitPoint.IsSholder_L = true;

			Desc.vPos = TargetCollision.RayDesc.OnCloiderPos;
			m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Blood"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &Desc);
			m_BlackBoard->Set_Data().iHp -= CPlayer_Manager::GetInstance()->Get_Damage();
			m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::DANIELA));
			m_pGameInstance->PlaySoundW(TEXT("Monster_hit_Gun.wav"), ENUM_CLASS(SOUND_CHANNEL::DANIELA), g_fBGMVolume - 0.9f);

			if (m_BlackBoard->Get_Data().fDamage_Cool <= 0.f)
				m_BlackBoard->Set_Data().iDamage += CPlayer_Manager::GetInstance()->Get_Damage();
			break;
		}
	}
}

void CDaniela::Daniela_Start()
{
	m_BlackBoard->Set_Data().IsIdle = false;
	m_bIsStart = true;
}

HRESULT CDaniela::Ready_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::RESIST);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::RESIST);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.4f, 0.4f, 0.4f);
	OBBDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Resist"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::RESIST)]), &OBBDesc)))
		return E_FAIL;

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


	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_SHOULDER_R);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(0.1f, 0.f, 0.f);
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_ARM_L"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::L_ARM)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_SHOULDER_L);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(-0.1f, 0.f, 0.f);
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_ARM_R"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::R_ARM)]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 6044;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;
	m_pNavigationCom->isMove(m_pTransformCom->Get_State(STATE::POSITION));

	return S_OK;
}

HRESULT CDaniela::Ready_PartObjects()
{
	CBody_Daniela::BODY_DESC BodyDesc{};
	BodyDesc.BlackBoard = m_BlackBoard;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();


	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Daniela"), &BodyDesc)))
		return E_FAIL;

	CBody_Daniela* pBody = static_cast<CBody_Daniela*>(Find_PartObject(TEXT("Part_Body")));

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

	return S_OK;
}

HRESULT CDaniela::Ready_Utility()
{
	m_BlackBoard = CBlackBoard<DANIELA_DATA>::Create();
	m_BlackBoard->Set_Data().iAnimState = &m_iAnimState;
	m_BlackBoard->Set_Data().szAnimTag = &m_szAnimTag;
	m_BlackBoard->Set_Data().bIsAnimFinsh = &m_bIsAnimFinsh;
	m_BlackBoard->Set_Data().bIsAnimLoop = &m_bIsAnimLoop;
	m_BlackBoard->Set_Data().szCulStateTag = &m_szCulStateTag;

	m_BlackBoard->Set_Data().iHp = 150;
	m_BlackBoard->Set_Data().iDamage = 0;
	m_BlackBoard->Set_Data().fDamage_Cool = 0.f;

	m_BlackBoard->Set_Data().IsHitPoint.IsBody = false;
	m_BlackBoard->Set_Data().IsHitPoint.IsHead = false;
	m_BlackBoard->Set_Data().IsHitPoint.IsSholder_L = false;
	m_BlackBoard->Set_Data().IsHitPoint.isSholder_R = false;

	m_BlackBoard->Set_Data().IsIdle = true;

	m_BlackBoard->Set_Data().fAttackCool = 0.f;
	m_BlackBoard->Set_Data().IsAttack = false;
	
	m_BlackBoard->Set_Data().fCriAttackCool = 0.f;
	m_BlackBoard->Set_Data().IsCriticalAttack = false;


	m_BlackBoard->Set_Data().iWeapon = 0;
	m_BlackBoard->Set_Data().iStartMotion = 0;

	m_BlackBoard->Set_Data().MonPos = m_pTransformCom->Get_WorldMatrixPtr();

	m_BlackBoard->Set_Data().isBogan = true;
	m_pBehaviorTree = CBehaviorTree_Daniela::Create(m_BlackBoard);




	return S_OK;
}

HRESULT CDaniela::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_Daniela::Create());
	Add_StateObject(TEXT("Critical_Attack"), CCritical_Attack_Daniela::Create());
	Add_StateObject(TEXT("Attack"), CAttack_Daniela::Create());
	Add_StateObject(TEXT("Chase"), CChase_Daniela::Create());
	Add_StateObject(TEXT("Damage"), CDamage_Daniela::Create());
	Add_StateObject(TEXT("Die"), CDie_Daniela::Create());

	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
	return S_OK;
}

HRESULT CDaniela::Ready_TriggerEvent()
{
	CTrigger::TRIGEER_DESC TriggerDesc;
	
	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Daniela_Start(); } };
	  
	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 3.f, 3.f, 3.f };
	TriggerDesc.vPos = _float3{ -28.32f, -0.69f, 41.31f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDaniela::Add_StateObject(const _wstring& strStateObjectTag, CMonState_Daniela* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CMonState_Daniela* CDaniela::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CDaniela::State_Change()
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

void CDaniela::Root_Move()
{
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
	m_pTransformCom->Is_Sliding(m_pNavigationCom);

	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));
}

void CDaniela::Collider_Update()
{
	_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
	_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

	for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
	{
		if (ENUM_CLASS(ColliderType_Mon::RESIST) == i)
		{
			m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());
			continue;
		}
		_matrix BoneMat = XMLoadFloat4x4(m_pColliderBone[i]);
		_vector vScale, vRot, vTrans;
		XMMatrixDecompose(&vScale, &vRot, &vTrans, BoneMat);

		_matrix WorldRotMat = XMMatrixRotationQuaternion(vRot);
		_matrix WorldTransMat = XMMatrixTranslationFromVector(vTrans);
		_matrix WorldMatrix = WorldRotMat * WorldTransMat * Worldmat;

		m_pColliderCom[i]->Update(WorldMatrix);
	}
}

CDaniela* CDaniela::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDaniela* pInstance = new CDaniela(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CDaniela"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDaniela::Clone(void* pArg)
{
	CDaniela* pInstance = new CDaniela(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CDaniela"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDaniela::Free()
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
}
