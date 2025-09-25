#include "pch.h"
#include "Monster_WereWolf.h"
#include "BehaviorTree_WereWolf.h"

#include "Body_WereWolf.h"

#include "Idle_WereWolf.h"
#include "Walk_WereWolf.h"
#include "Attack_WereWolf.h"
#include "Event_1_WereWolf.h"
#include "Event_2_WereWolf.h"
#include "Event_3_WereWolf.h"
#include "Damage_WereWolf.h"
#include "Die_WereWolf.h"

CMonster_WereWolf::CMonster_WereWolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{
}

CMonster_WereWolf::CMonster_WereWolf(const CMonster_WereWolf& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CMonster_WereWolf::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMonster_WereWolf::Initialize(void* pArg)
{
	m_iAnimState = ENUM_CLASS(CMonster_WereWolf::ANIM_STATE::NORMAL);
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
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::ATTACK_L)] = m_pBodyObject->Get_BoneMatrix(TEXT("L_FrontLeg2"));
	m_pColliderBone[ENUM_CLASS(ColliderType_Mon::ATTACK_R)] = m_pBodyObject->Get_BoneMatrix(TEXT("R_FrontLeg2"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-64.51f, -1.11f, 38.16f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));
	return S_OK;
}

void CMonster_WereWolf::Priority_Update(_float fTimeDelta)
{
	if (!m_bIsStart)
		return;

	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);
}

void CMonster_WereWolf::Update(_float fTimeDelta)
{
	if (!m_bIsStart)
		return;

	if (m_BlackBoard->Get_Data().fAttackCool > 0.f)
		m_BlackBoard->Set_Data().fAttackCool -= fTimeDelta;

	m_pBehaviorTree->Update();

	State_Change();
	m_pCulStateObject->Update(this, fTimeDelta);

	Root_Move();

	m_pBodyObject->Update(fTimeDelta);
	Collider_Update();
}

void CMonster_WereWolf::Late_Update(_float fTimeDelta)
{
	if (!m_bIsStart)
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);
	for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
	{
#ifdef _DEBUG
		m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
#endif // DEBUG
		if (i == ENUM_CLASS(ColliderType_Mon::ATTACK_L) || i == ENUM_CLASS(ColliderType_Mon::ATTACK_R))
			continue;

		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
			return;
	}
}

HRESULT CMonster_WereWolf::Render()
{
	return S_OK;
}

void CMonster_WereWolf::Event3_Create()
{
	CTrigger::TRIGEER_DESC TriggerDesc;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_3(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 1.f, 1.f, 1.f };
	TriggerDesc.vPos = _float3{ -46.91f, -3.63f, 29.99f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return;

}

void CMonster_WereWolf::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

void CMonster_WereWolf::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
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
			if (m_BlackBoard->Set_Data().IsMonDamage == true)
			{
				m_BlackBoard->Set_Data().iHp -= CPlayer_Manager::GetInstance()->Get_Damage();
				m_BlackBoard->Set_Data().iDamage += CPlayer_Manager::GetInstance()->Get_Damage();

				Desc.vPos = TargetCollision.RayDesc.OnCloiderPos;
				m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Blood"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &Desc);

				m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::MONSTER_2));
				m_pGameInstance->PlaySoundW(TEXT("Monster_hit_Gun.wav"), ENUM_CLASS(SOUND_CHANNEL::MONSTER_2), g_fBGMVolume - 0.7f);
			}
			else
			{
				m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
				m_pGameInstance->PlaySoundW(TEXT("ricochet.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.4f);
			}
			break;
		}
	}
}

void CMonster_WereWolf::Target_LookTurn(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();

	_vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK), 0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CMonster_WereWolf::Attack_Collision()
{
	if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[ENUM_CLASS(ColliderType_Mon::ATTACK_L)])))
		return;

	if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[ENUM_CLASS(ColliderType_Mon::ATTACK_R)])))
		return;
}

void CMonster_WereWolf::Event_1()
{
	m_bIsStart = true;
	m_BlackBoard->Set_Data().IsEvent_1 = true;
	m_BlackBoard->Set_Data().IsEvent_2 = false;
	m_BlackBoard->Set_Data().IsEvent_3 = false;
}

void CMonster_WereWolf::Event_2()
{
	m_bIsStart = true;
	m_BlackBoard->Set_Data().IsEvent_1 = false;
	m_BlackBoard->Set_Data().IsEvent_2 = true;
	m_BlackBoard->Set_Data().IsEvent_3 = false;

	m_pTransformCom->Set_State(STATE::POSITION, _vector{ -47.46f, -3.63f, 30.14f, 1.f });
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));
	m_pNavigationCom->Set_CellIndex(5168);
	m_pBodyObject->Reset_MovePos();

}

void CMonster_WereWolf::Event_3()
{
	m_bIsStart = true;
	m_BlackBoard->Set_Data().IsEvent_1 = false;
	m_BlackBoard->Set_Data().IsEvent_2 = false;
	m_BlackBoard->Set_Data().IsEvent_3 = true;

	m_pTransformCom->Set_State(STATE::POSITION, _vector{ -49.f, -3.63f, 37.14f, 1.f });
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));
	m_pBodyObject->Reset_MovePos();
}

HRESULT CMonster_WereWolf::Ready_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::RESIST);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::RESIST);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(1.f, 1.f, 1.f);
	OBBDesc.vCenter = _float3(0.f, 0.5f, 1.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Resist"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::RESIST)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.7f, 0.4f, 1.f);
	OBBDesc.vCenter = _float3(0.f, 0.f, -0.7f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::Body)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ATTACK);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.2f, 0.2f, 0.7f);
	OBBDesc.vCenter = _float3(0.f, 0.f, -0.3f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Attack_L"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::ATTACK_L)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ATTACK);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.2f, 0.2f, 0.7f);
	OBBDesc.vCenter = _float3(0.f, 0.f, -0.3f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Attack_R"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::ATTACK_R)]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 4995;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonster_WereWolf::Ready_PartObjects()
{
	CBody_WereWolf::BODY_DESC BodyDesc{};
	BodyDesc.pBalckBoard = m_BlackBoard;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_2"), &BodyDesc)))
		return E_FAIL;

	CBody_WereWolf* pBody = static_cast<CBody_WereWolf*>(Find_PartObject(TEXT("Part_Body")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pBodyObject = pBody;
	Safe_AddRef(m_pBodyObject);

	return S_OK;
}

HRESULT CMonster_WereWolf::Ready_Utility()
{
	m_BlackBoard = CBlackBoard<WEREWOLF_DATA>::Create();
	m_BlackBoard->Set_Data().bIsAnimFinsh = &m_bIsAnimFinsh;
	m_BlackBoard->Set_Data().bIsAnimLoop = &m_bIsAnimLoop;
	m_BlackBoard->Set_Data().iAnimState = &m_iAnimState;
	m_BlackBoard->Set_Data().szAnimTag = &m_szAnimTag;
	m_BlackBoard->Set_Data().szCulStateTag = &m_szCulStateTag;
	m_BlackBoard->Set_Data().iHp = 100;
	m_BlackBoard->Set_Data().iDamage = 0;

	m_BlackBoard->Set_Data().IsAttack = false;
	m_BlackBoard->Set_Data().fAttackCool = 0.f;
	m_BlackBoard->Set_Data().IsChase = false;
	m_BlackBoard->Set_Data().MonPos = m_pTransformCom->Get_WorldMatrixPtr();

	m_BlackBoard->Set_Data().IsEvent_1 = false;
	m_BlackBoard->Set_Data().IsEvent_2 = false;
	m_BlackBoard->Set_Data().IsEvent_3 = false;
	m_BlackBoard->Set_Data().fNoies = 0.f;
	m_BlackBoard->Set_Data().IsMonDamage = false;
	m_pBehaviorTree = CBehaviorTree_WereWolf::Create(m_BlackBoard);

	return S_OK;
}

HRESULT CMonster_WereWolf::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_WereWolf::Create());
	Add_StateObject(TEXT("Attack"), CAttack_WereWolf::Create());
	Add_StateObject(TEXT("Walk"), CWalk_WereWolf::Create());
	Add_StateObject(TEXT("Event_1"), CEvent_1_WereWolf::Create());
	Add_StateObject(TEXT("Event_2"), CEvent_2_WereWolf::Create());
	Add_StateObject(TEXT("Event_3"), CEvent_3_WereWolf::Create());
	Add_StateObject(TEXT("Damage"), CDamage_WereWolf::Create());
	Add_StateObject(TEXT("Die"), CDie_WereWolf::Create());
	
	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
	return S_OK;
}

HRESULT CMonster_WereWolf::Ready_TriggerEvent()
{
	CTrigger::TRIGEER_DESC TriggerDesc;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_1(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 4.f, 4.f, 4.f };
	TriggerDesc.vPos = _float3{ -55.06f, -3.63f, 39.44f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_2(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 1.f, 1.f, 1.f };
	TriggerDesc.vPos = _float3{ -45.24f, -3.63f, 29.99f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_WereWolf::Add_StateObject(const _wstring& strStateObjectTag, CMonState_WereWolf* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CMonState_WereWolf* CMonster_WereWolf::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CMonster_WereWolf::State_Change()
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

void CMonster_WereWolf::Root_Move()
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

void CMonster_WereWolf::Collider_Update()
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

CMonster_WereWolf* CMonster_WereWolf::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_WereWolf* pInstance = new CMonster_WereWolf(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CMonster_WereWolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_WereWolf::Clone(void* pArg)
{
	CMonster_WereWolf* pInstance = new CMonster_WereWolf(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMonster_WereWolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_WereWolf::Free()
{
	__super::Free();
	for (auto& Pair : m_StateObjects)
		Safe_Release(Pair.second);

	m_StateObjects.clear();

	Safe_Release(m_pCulStateObject);
	Safe_Release(m_pBodyObject);
	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_BlackBoard);
	Safe_Release(m_pBehaviorTree);
}
