#include "pch.h"
#include "Alcina.h"
#include "Player_Manager.h"
#include "Body_Alcina.h"
#include "BehaviorTree_Alcina.h"
#include "BlackBoard.h"
#include "MonState_Alcina.h"

#include "Walk_Alcina.h"
#include "Idle_Alcina.h"
#include "Attack1_Alcina.h"
#include "Attack2_Alcina.h"
#include "Attack3_Alcina.h"
#include "Event1_Alcina.h"

#include "Damage_Alcina.h"
#include "Die_Alcina.h"

CAlcina::CAlcina(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{
}

CAlcina::CAlcina(const CAlcina& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CAlcina::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CAlcina::Initialize(void* pArg)
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


	m_pColliderBone[ColliderType_Mon::Body] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[ColliderType_Mon::Head] = m_pBodyObject->Get_BoneMatrix(TEXT("Head"));

	m_pColliderBone[ColliderType_Mon::ATTACK_L] = m_pBodyObject->Get_BoneMatrix(TEXT("L_MiddleNail_1"));
	m_pColliderBone[ColliderType_Mon::ATTACK_R] = m_pBodyObject->Get_BoneMatrix(TEXT("R_MiddleNail_1"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-10.87f, -8.68f, 55.79f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	m_pGameInstance->Subscribe<Event_OpenDoor>([&](const Event_OpenDoor& e) {m_bIsStart = true; });

	return S_OK;
}

void CAlcina::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(DIK_H))
	{
		m_pGameInstance->Publish(Event_OpenDoor{ true, 1.f });
	}
	if (!m_bIsStart)
		return;
		
	m_bIsHeadShot = false;
	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);

	if (m_BlackBoard->Get_Data().fAttack2Cool > 0.f)
	{
		m_BlackBoard->Set_Data().fAttack2Cool -= fTimeDelta;
	}

	if (m_BlackBoard->Get_Data().bIsFly == true)
		m_pEffect->Priority_Update(fTimeDelta);
	if (m_BlackBoard->Get_Data().bIsSpawnFly == true)
		m_pSpawnEffect->Priority_Update(fTimeDelta);
}

void CAlcina::Update(_float fTimeDelta)
{
	if (!m_bIsStart)
		return;

	m_pBehaviorTree->Update();
	State_Change();
	m_pCulStateObject->Update(this, fTimeDelta);
	
	m_pBodyObject->Update(fTimeDelta);

	//위치 보정
	Root_Move();

	Collider_Update();

	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 2.f);

	if (m_BlackBoard->Get_Data().bIsFly == true)
	{
		m_pGameInstance->PlayLoopSound(TEXT("Fly.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA_EFFECT), g_fBGMVolume - 0.3f);

		m_pEffect->Set_Potion(vPos);
		m_pEffect->Update(fTimeDelta);
	}
	else
		m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA_EFFECT));


	if (m_BlackBoard->Get_Data().bIsSpawnFly == true)
	{
		m_pSpawnEffect->Set_Potion(vPos);
		m_pSpawnEffect->Update(fTimeDelta);
	}
}

void CAlcina::Late_Update(_float fTimeDelta)
{
	if (!m_bIsStart)
		return;

	if (m_BlackBoard->Get_Data().fNoies < 0.8f)
	{
		for (_int i = 0; i < ColliderType_Mon::End; ++i)
		{
			if (i == ColliderType_Mon::ATTACK_L || i == ColliderType_Mon::ATTACK_R)
				continue;

			if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
				return;
		}
	}
	Update_Effect(fTimeDelta);
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::EFFECT, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);
#ifdef _DEBUG
	for (_int i = 0; i < ColliderType_Mon::End; ++i)
	{
		m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
	}
#endif
	if (m_BlackBoard->Get_Data().bIsFly == true)
		m_pEffect->Late_Update(fTimeDelta);

	if (m_BlackBoard->Get_Data().bIsSpawnFly == true)
		m_pSpawnEffect->Late_Update(fTimeDelta);

}

HRESULT CAlcina::Render()
{
	m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	m_pTrailTexCom->Bind_Shader_Resource(m_pShaderCom, "g_DiffuseTexture", 1);
	m_pShaderCom->Begin(0);

	for (_int i = 0; i < 4; ++i)
	{
		m_pTrailCom[i]->Bind_Resources();
		m_pTrailCom[i]->Render();
	}
	return S_OK;
}

void CAlcina::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

CTransform* CAlcina::Get_TransForm()
{
	return m_pTransformCom;
}

void CAlcina::Target_LookTurn(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();

	_vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK), 0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CAlcina::Target_LookAt()
{
	m_pTransformCom->LookAt(CPlayer_Manager::GetInstance()->Get_PlayerPos());
}

void CAlcina::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
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
				m_bIsHeadShot = true;
			if (true == m_bIsHeadShot)
			{
				m_BlackBoard->Set_Data().iHp -= _int(CPlayer_Manager::GetInstance()->Get_Damage() * 1.3f);
				m_BlackBoard->Set_Data().iDamage += _int(CPlayer_Manager::GetInstance()->Get_Damage() * 1.3f);
			}
			else
			{
				m_BlackBoard->Set_Data().iHp -= CPlayer_Manager::GetInstance()->Get_Damage();
				m_BlackBoard->Set_Data().iDamage += CPlayer_Manager::GetInstance()->Get_Damage();
			}
			
			m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::ALCINA));
			m_pGameInstance->PlaySoundW(TEXT("Monster_hit_Gun.wav"), ENUM_CLASS(SOUND_CHANNEL::ALCINA), g_fBGMVolume - 0.7f);
			Desc.vPos = TargetCollision.RayDesc.OnCloiderPos;
			m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Blood"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &Desc);
			break;
		case ENUM_CLASS(OBJECT_TYPE::PLAYER):
			CPlayer_Manager::GetInstance()->Player_Hp(-25);
			break;
		}
	}

}

void CAlcina::Attack_Collision(_bool isRight)
{

		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[ColliderType_Mon::ATTACK_R])))
			return;

		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[ColliderType_Mon::ATTACK_L])))
			return;

}

_vector CAlcina::Bone_WorldTransform(const _wstring pBoneName)
{
	_float4x4 BoneMatrix = *m_pBodyObject->Get_BoneMatrix(pBoneName);
	_vector vBonePos = { BoneMatrix._41, BoneMatrix._42, BoneMatrix._43, 1.f };

	vBonePos = XMVector3TransformCoord(vBonePos, m_pTransformCom->Get_WorldMatrix());

	return vBonePos;
}

void CAlcina::Trail_On(_bool m_IsRight)
{
	m_bIsTrail = true;
	m_bIsRight = m_IsRight;
	for (_int i = 0; i < 4; ++i)
	{
		m_pTrailCom[i]->Reset();
	}
}

void CAlcina::Trail_Off()
{
	m_bIsTrail = false;
}

void CAlcina::Spawn_EffectReset()
{
	m_pSpawnEffect->Reset();
}

HRESULT CAlcina::Ready_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::RESIST);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::RESIST);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.4f, 1.f, 0.4f);
	OBBDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Resist"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::RESIST]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.3f, 1.1f, 0.3f);
	OBBDesc.vCenter = _float3(0.f, -0.25f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::Body]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_HEAD);
	OBBDesc.vExtents = _float3(0.12f, 0.15f, 0.12f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::Head]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ATTACK);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.7f, 0.5f, 0.2f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Attack_R"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::ATTACK_R]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ATTACK);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.7f, 0.5f, 0.2f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Attack_L"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::ATTACK_L]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 6932;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_Trail"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_TrailCom_0"), reinterpret_cast<CComponent**>(&m_pTrailCom[0]))))
		return E_FAIL;
	
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_TrailCom_1"), reinterpret_cast<CComponent**>(&m_pTrailCom[1]))))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_TrailCom_2"), reinterpret_cast<CComponent**>(&m_pTrailCom[2]))))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_TrailCom_3"), reinterpret_cast<CComponent**>(&m_pTrailCom[3]))))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Trail"),
		TEXT("Com_TrailTexCom"), reinterpret_cast<CComponent**>(&m_pTrailTexCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CAlcina::Ready_PartObjects()
{
	CBody_Alcina::BODY_DESC BodyDesc{};
	BodyDesc.pBalckBoard = m_BlackBoard;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Alcina"), &BodyDesc)))
		return E_FAIL;


	CBody_Alcina* pBody = static_cast<CBody_Alcina*>(Find_PartObject(TEXT("Part_Body")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pBodyObject = pBody;
	Safe_AddRef(m_pBodyObject);

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

HRESULT CAlcina::Ready_Utility()
{
	m_BlackBoard = CBlackBoard<ALCHINA_DATA>::Create();
	m_BlackBoard->Set_Data().bIsAnimFinsh = &m_bIsAnimFinsh;
	m_BlackBoard->Set_Data().bIsAnimLoop = &m_bIsAnimLoop;
	m_BlackBoard->Set_Data().iAnimState = &m_iAnimState;
	m_BlackBoard->Set_Data().szAnimTag = &m_szAnimTag;
	m_BlackBoard->Set_Data().szCulStateTag = &m_szCulStateTag; 
	
	m_BlackBoard->Set_Data().isBogan = true;
	m_BlackBoard->Set_Data().iHp = 200;
	m_BlackBoard->Set_Data().iDamage = 0;

	m_BlackBoard->Set_Data().IsAttack = false;
	m_BlackBoard->Set_Data().IsPage2 = false;

	m_BlackBoard->Set_Data().fAttack2Cool = 0.f;

	m_BlackBoard->Set_Data().IsChase = false;
	m_BlackBoard->Set_Data().IsEvent_1 = false;

	m_BlackBoard->Set_Data().MonPos = m_pTransformCom->Get_WorldMatrixPtr();
	m_BlackBoard->Set_Data().fNoies = 0.f;
	m_pBehaviorTree = CBehaviorTree_Alcina::Create(m_BlackBoard);

	m_BlackBoard->Set_Data().bIsFly = false;
	m_BlackBoard->Set_Data().bIsSpawnFly = false;

	return S_OK;
}

HRESULT CAlcina::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_Alcina::Create());
	Add_StateObject(TEXT("Attack1"), CAttack1_Alcina::Create());
	Add_StateObject(TEXT("Attack2"), CAttack2_Alcina::Create());
	Add_StateObject(TEXT("Attack3"), CAttack3_Alcina::Create());

	Add_StateObject(TEXT("Event1"), CEvent1_Alcina::Create());

	Add_StateObject(TEXT("Walk"), CWalk_Alcina::Create());
	Add_StateObject(TEXT("Damage"), CDamage_Alcina::Create());
	Add_StateObject(TEXT("Die"), CDie_Alcina::Create());

	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
	return S_OK;
}

HRESULT CAlcina::Ready_TriggerEvent()
{
	CTrigger::TRIGEER_DESC TriggerDesc;

	TriggerDesc.eType = TRIGGER_TYPE::PLAYER;
	TriggerDesc.eObjType = OBJECT_TYPE::PLAYER;
	TriggerDesc.TriggerEvent = { [&]() {return Event_Start(); } };

	TriggerDesc.vCenter = _float3{ 0.f, 0.f, 0.f };
	TriggerDesc.vExtents = _float3{ 1.f, 1.f, 1.f };
	TriggerDesc.vPos = _float3{ -14.60, -8.68f, 44.97f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Trigger"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CAlcina::Add_StateObject(const _wstring& strStateObjectTag, CMonState_Alcina* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CMonState_Alcina* CAlcina::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CAlcina::State_Change()
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


void CAlcina::Root_Move()
{
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
}

void CAlcina::Collider_Update()
{
	_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
	_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

	for (_int i = 0; i < ColliderType_Mon::End; ++i)
	{
		if (ColliderType_Mon::RESIST == i)
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

void CAlcina::Event_Start()
{
	m_BlackBoard->Set_Data().IsChase = true;
	m_BlackBoard->Set_Data().fAttack2Cool = 10.f;
	m_pGameInstance->Publish(Event_OpenDoor{ false, 2.f });
	m_pGameInstance->StopAll();
	m_pGameInstance->PlayBGM(TEXT("Boss_Bgm_2.wav"), g_fBGMVolume - 0.1f);
}

void CAlcina::Update_Effect(_float fTimeDelta)
{
	for (_int i = 0; i < 4; ++i)
	{
		m_pTrailCom[i]->Update_Trail(fTimeDelta);
	}
	if (m_bIsTrail == true)
	{
		_float3 vTopPos{}, vBottomPos;
		if (m_bIsRight)
		{
			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("R_IndexNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("R_IndexNail_0")));
			m_pTrailCom[0]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);

			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("R_MiddleNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("R_MiddleNail_0")));
			m_pTrailCom[1]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);

			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("R_PinkyNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("R_PinkyNail_0")));
			m_pTrailCom[2]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);

			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("R_RingNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("R_RingNail_0")));
			m_pTrailCom[3]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);
		}
		else
		{
			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("L_PinkyNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("L_PinkyNail_0")));
			m_pTrailCom[0]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);

			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("L_RingNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("L_RingNail_0")));
			m_pTrailCom[1]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);

			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("L_MiddleNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("L_MiddleNail_0")));
			m_pTrailCom[2]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);

			XMStoreFloat3(&vTopPos, Bone_WorldTransform(TEXT("L_IndexNail_1")));
			XMStoreFloat3(&vBottomPos, Bone_WorldTransform(TEXT("L_IndexNail_0")));
			m_pTrailCom[3]->Add_Postion(vTopPos, vBottomPos, fTimeDelta);
		}
	}
}

CAlcina* CAlcina::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAlcina* pInstance = new CAlcina(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CAlcina"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAlcina::Clone(void* pArg)
{
	CAlcina* pInstance = new CAlcina(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CAlcina"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlcina::Free()
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

	for (_int i = 0; i < 4; ++i)
		Safe_Release(m_pTrailCom[i]);
	Safe_Release(m_pTrailTexCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pEffect);
	Safe_Release(m_pSpawnEffect);

}
