#include "pch.h"
#include "Bela.h"
#include "Player_Manager.h"

#include "Body_Bela.h"
#include "WeaponObject.h"

#include "Idle_Bela.h"
#include "Die_Bela.h"
#include "Damage_Bela.h"
#include "Chase_Bela.h"
#include "Attack_Bela.h"
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
	m_iHp = 10000;
	m_iAnimState = ENUM_CLASS(BOSS_SISTER::NORMAL);
	m_szAnimTag = "Idle_Freezes";

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;

	m_pColliderBone[ColliderType_Mon::Body] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[ColliderType_Mon::Head] = m_pBodyObject->Get_BoneMatrix(TEXT("Head"));
	m_pColliderBone[ColliderType_Mon::L_ARM] = m_pBodyObject->Get_BoneMatrix(TEXT("L_UpperArm"));
	m_pColliderBone[ColliderType_Mon::R_ARM] = m_pBodyObject->Get_BoneMatrix(TEXT("R_UpperArm"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-60.66f, -8.68f, 30.52f, 1.f));


	return S_OK;
}

void CBela::Priority_Update(_float fTimeDelta)
{
	m_State = {};

	m_pBodyObject->Priority_Update(fTimeDelta);
	m_pWeaponObject->Priority_Update(fTimeDelta);
}

void CBela::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));


	//스테이터스 업데이트
	State_Check();
	m_pCulStateObject->Update(this, fTimeDelta);
	State_Change();

	m_pBodyObject->Update(fTimeDelta);
	m_pWeaponObject->Update(fTimeDelta);

	//위치 보정
	Root_Move();
	Collider_Update();
}

void CBela::Late_Update(_float fTimeDelta)
{
	for (_int i = 0; i < ColliderType_Mon::End; ++i)
	{
		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
			return;
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);

	m_pWeaponObject->Late_Update(fTimeDelta);
}

HRESULT CBela::Render()
{
#ifdef _DEBUG
	for (_int i = 0; i < ColliderType_Mon::End; ++i)
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

void CBela::IsDamage()
{
	if (m_IsHitPoint.IsHead)
		m_iHp -= CPlayer_Manager::GetInstance()->Get_Damage() * 1.3f;
	else
		m_iHp -= CPlayer_Manager::GetInstance()->Get_Damage();
}
void CBela::OnCollision(_uint MyObjectType, _uint TargetObjectType)
{
	switch (TargetObjectType)
	{
	case ENUM_CLASS(OBJECT_TYPE::RAY):

		if (MyObjectType == ENUM_CLASS(OBJECT_TYPE::MON_HEAD))
			m_IsHitPoint.IsHead = true;
		if (MyObjectType == ENUM_CLASS(OBJECT_TYPE::MON_BODY))
			m_IsHitPoint.IsBody = true;
		if (MyObjectType == ENUM_CLASS(OBJECT_TYPE::MON_SHOULDER_R))
			m_IsHitPoint.isSholder_R = true;
		if (MyObjectType == ENUM_CLASS(OBJECT_TYPE::MON_SHOULDER_L))
			m_IsHitPoint.IsSholder_L = true;

		m_bIsDamage = true;

		break;
	}
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
		TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::Body]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_HEAD);
	OBBDesc.vExtents = _float3(0.12f, 0.15f, 0.12f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::Head]), &OBBDesc)))
		return E_FAIL;
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(0.1f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_ARM_L"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::L_ARM]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(-0.1f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_ARM_R"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::R_ARM]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 126;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CBela::Ready_PartObjects()
{
	CBody_Bela::BODY_DESC BodyDesc{};
	BodyDesc.pAnimState = &m_iAnimState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pAnimTag = &m_szAnimTag;
	BodyDesc.pIsAnimLoop = &m_bIsAnimLoop;
	BodyDesc.pIsAnimFinsh = &m_bIsAnimFinsh;

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

	CWeaponObject* pWeaponObject = static_cast<CWeaponObject*>(Find_PartObject(TEXT("Part_Shotel")));

	if (pWeaponObject == nullptr)
		return E_FAIL;

	m_pWeaponObject = pWeaponObject;
	Safe_AddRef(m_pWeaponObject);

	return S_OK;
}

HRESULT CBela::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_Bela::Create());
	Add_StateObject(TEXT("Attack"), CAttack_Bela::Create());
	Add_StateObject(TEXT("Chase"), CChase_Bela::Create());
	Add_StateObject(TEXT("Damage"), CDamage_Bela::Create());
	Add_StateObject(TEXT("Die"), CDie_Bela::Create());

	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
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

void CBela::State_Check()
{
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_float fDis = {};
	vPlayerPos = XMVector3Length(m_pTransformCom->Get_State(STATE::POSITION) - vPlayerPos);
	XMStoreFloat(&fDis, vPlayerPos);

	if (fDis <= 3.f)
		m_State.isAttack = true;
	if (fDis <= 6.f)
		m_State.isChase = true;
	if (m_bIsDamage)
	{
		m_bIsDamage = false;
		m_State.isDamage = true;
	}
}

void CBela::State_Change()
{
	if (m_iHp <= 0)
		m_szCulStateTag = TEXT("Die");

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
	vWorldRot = XMQuaternionMultiply(vMoveRot, vWorldRot);
	vWorldRot = XMQuaternionNormalize(vWorldRot);

	_matrix ScaleMat = XMMatrixScalingFromVector(vScale);
	_matrix Movemat = XMMatrixTranslationFromVector(vWorldTrans);
	_matrix RotMat = XMMatrixRotationQuaternion(vWorldRot);

	_float4x4 WorldMatrix = {};
	XMStoreFloat4x4(&WorldMatrix, XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vWorldRot, vWorldTrans));

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	if (false == m_pNavigationCom->isMove(m_pTransformCom->Get_State(STATE::POSITION)))
		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(vPos, 1.f));
}

void CBela::Collider_Update()
{
	_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
	_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

	for (_int i = 0; i < ColliderType_Mon::End; ++i)
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
}
