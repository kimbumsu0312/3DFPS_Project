#include "pch.h"
#include "Monster_Normal.h"
#include "Player_Manager.h"

#include "Body_NorMon.h"
#include "WeaponObject.h"

#include "Stand_NorMon_1.h"
#include "Scouting_NorMon_1.h"
#include "Damage_NorMon_1.h"
#include "Chase_NorMon_1.h"
#include "Attack_NorMon_1.h"
#include "Die_NorMon_1.h"

CMonster_Normal::CMonster_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingContainer(pDevice, pContext)
{
}

CMonster_Normal::CMonster_Normal(const CMonster_Normal& Prototype) : CPoolingContainer(Prototype)
{
}

HRESULT CMonster_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Normal::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scale(_float3(1.f, 1.f, 1.f));

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;

	m_pColliderBone[ColliderType_Mon::BODY] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[ColliderType_Mon::Head] = m_pBodyObject->Get_BoneMatrix(TEXT("Head"));
	m_pColliderBone[ColliderType_Mon::Hand] = m_pBodyObject->Get_BoneMatrix(TEXT("L_Hand"));

	return S_OK;
}

void CMonster_Normal::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->PrePostion_Update();
	m_NorMonState = {};
	
	m_pBodyObject->Priority_Update(fTimeDelta);
	if(m_pWeaponObject != nullptr)
		m_pWeaponObject->Priority_Update(fTimeDelta);
}

void CMonster_Normal::Update(_float fTimeDelta)
{
	//스테이터스 업데이트
	State_Check();
	m_pCulStateObject->Update(this, fTimeDelta);

	State_Change();
	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));

	//모델 랜더
	Root_Move();
	m_pBodyObject->Update(fTimeDelta);
	
	//무기 랜더
	if (m_pWeaponObject != nullptr)
		m_pWeaponObject->Update(fTimeDelta);
	
	//위치 보정
	Collider_Update();
}

void CMonster_Normal::Late_Update(_float fTimeDelta)
{
	for (_int i = 0; i < ColliderType_Mon::END; ++i)
	{
		if (i == ColliderType_Mon::Hand)
			continue;

		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
			return;
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;


	m_pBodyObject->Late_Update(fTimeDelta);
	
	if (m_pWeaponObject != nullptr)
		m_pWeaponObject->Late_Update(fTimeDelta);

}

HRESULT CMonster_Normal::Render()
{
#ifdef _DEBUG
	for (_int i = 0; i < ColliderType_Mon::END; ++i)
	{
		if (m_pWeaponObject != nullptr && i == ColliderType_Mon::Hand)
			continue;

		m_pColliderCom[i]->Render();
	}

#endif
	return S_OK;
}

void CMonster_Normal::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

const _int CMonster_Normal::Get_CulNaviIndex()
{
	return m_pNavigationCom->Get_CulIndex();
}

void CMonster_Normal::Target_LookAt(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();

	_vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK), 0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CMonster_Normal::Attack_Collision()
{
	if (m_pWeaponObject == nullptr)
	{
		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[ColliderType_Mon::Hand])))
			return;
	}
	else
	{
		if(FAILED(m_pWeaponObject->Add_Collider()))
			return;
	}
}

void CMonster_Normal::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
	{
		if (TargetCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
			m_pTransformCom->Is_Sliding(m_pNavigationCom, XMLoadFloat3(&TargetCollision.pCollider->Get_Intersect_Normal()));
	}

	switch (TargetCollision.iObjType)
	{
	case ENUM_CLASS(OBJECT_TYPE::RAY):
		(MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::MON_HEAD)) ? m_bIsHeadShot = true : m_bIsHeadShot = false;
		m_bIsDamage = true;

		if(m_bIsHeadShot)
			m_iHp -= _int(CPlayer_Manager::GetInstance()->Get_Damage() * 1.3f);
		else
			m_iHp -= CPlayer_Manager::GetInstance()->Get_Damage();
		break;
	}
}

HRESULT CMonster_Normal::Initialize_Pool(void* pArg)
{
	POOLMONDESC* pDesc = static_cast<POOLMONDESC*>(pArg);

	m_iHp = 100;
	m_iAnimState = pDesc->iAnimState;
	m_szAnimTag = pDesc->szAnimTag;

	switch (pDesc->iWeponType)
	{
	case ENUM_CLASS(NORMAL_MON_WEAPON::SWORD):
		m_pWeaponObject = static_cast<CWeaponObject*>(Find_PartObject(TEXT("Part_Sword")));
		Safe_AddRef(m_pWeaponObject);
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::SWORD);
		break;
	case ENUM_CLASS(NORMAL_MON_WEAPON::SHOTEL):
		m_pWeaponObject = static_cast<CWeaponObject*>(Find_PartObject(TEXT("Part_Shotel")));
		Safe_AddRef(m_pWeaponObject);
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::SHOTEL);
		break;
	case ENUM_CLASS(NORMAL_MON_WEAPON::HALBERD):
		m_pWeaponObject = static_cast<CWeaponObject*>(Find_PartObject(TEXT("Part_Halberd")));
		Safe_AddRef(m_pWeaponObject);
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::HALBERD);
		break;
	case ENUM_CLASS(NORMAL_MON_WEAPON::END):
		m_pWeaponObject = nullptr;
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::END);
		break;

	}
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPostion, 1.f));
	m_pNavigationCom->Set_CellIndex(pDesc->iCellIndex);

	m_pCulStateObject = Find_StateObject(pDesc->szState);
	Safe_AddRef(m_pCulStateObject);

	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-53.29f, -8.68f, 28.24f, 1.f));

	return S_OK;
}

void CMonster_Normal::Return_Pool()
{
	m_iHp = 1;
	m_bIsDead = false;
}

HRESULT CMonster_Normal::Ready_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::RESIST);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::RESIST);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.4f, 0.4f, 0.4f);
	OBBDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Resist"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::RESIST]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.2f, 0.63f, 0.2f);
	OBBDesc.vCenter = _float3(0.f, 0.f, -0.2f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::BODY]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_HEAD);
	OBBDesc.vExtents = _float3(0.15f, 0.15f, 0.15f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::Head]), &OBBDesc)))
		return E_FAIL;
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::WEAPON);
	OBBDesc.vExtents = _float3(0.2f, 0.07f, 0.07f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Hand"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::Hand]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = -1;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Normal::Ready_PartObjects()
{
	CBody_NorMon::BODY_DESC BodyDesc{};
	BodyDesc.pAnimState = &m_iAnimState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pAnimTag = &m_szAnimTag;
	BodyDesc.pIsAnimLoop = &m_bIsAnimLoop;
	BodyDesc.pIsAnimFinsh = &m_bIsAnimFinsh;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1"), &BodyDesc)))
		return E_FAIL;

	CBody_NorMon* pBody = static_cast<CBody_NorMon*>(Find_PartObject(TEXT("Part_Body")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pBodyObject = pBody;
	Safe_AddRef(m_pBodyObject);

	CWeaponObject::WEAPON_DESC WeaponDesc{};
	WeaponDesc.pCulStateTag = &m_szCulStateTag;
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_Wep"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_Sword"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1_Sword"), &WeaponDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Halberd"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1_Halberd"), &WeaponDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Shotel"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1_Shotel"), &WeaponDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Normal::Ready_StateObjects()
{
	Add_StateObject(TEXT("Stand"), CStand_NorMon_1::Create());
	Add_StateObject(TEXT("Attack"), CAttack_NorMon_1::Create());
	Add_StateObject(TEXT("Chase"), CChase_NorMon_1::Create());
	Add_StateObject(TEXT("Scouting"), CScouting_NorMon_1::Create());
	Add_StateObject(TEXT("Damage"), CDamage_NorMon_1::Create());
	Add_StateObject(TEXT("Die"), CDie_Normon_1::Create());
	return S_OK;
}

HRESULT CMonster_Normal::Add_StateObject(const _wstring& strStateObjectTag, CMonState_Normal* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CMonState_Normal* CMonster_Normal::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CMonster_Normal::State_Check()
{
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_float fDis = {};
	vPlayerPos = XMVector3Length(m_pTransformCom->Get_State(STATE::POSITION) - vPlayerPos);
	XMStoreFloat(&fDis, vPlayerPos);

	if(fDis <= 3.f)
		m_NorMonState.isAttack = true;
	if (fDis <= 6.f)
		m_NorMonState.isChase = true;
	if (m_bIsDamage)
	{
		m_bIsDamage = false;
		m_NorMonState.isDamage = true;
	}
	m_NorMonState.iWeponType = m_iWeaponType;
}

void CMonster_Normal::State_Change()
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

void CMonster_Normal::Root_Move()
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
	vWorldRot = XMQuaternionMultiply(vMoveRot, vWorldRot);
	//vWorldRot = XMQuaternionNormalize(vWorldRot);

	_matrix ScaleMat = XMMatrixScalingFromVector(vScale);
	_matrix Movemat = XMMatrixTranslationFromVector(vWorldTrans);
	_matrix RotMat = XMMatrixRotationQuaternion(vWorldRot);

	_float4x4 WorldMatrix = {};
	XMStoreFloat4x4(&WorldMatrix, XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vWorldRot, vWorldTrans));

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	m_pTransformCom->Is_Sliding(m_pNavigationCom);
}

void CMonster_Normal::Collider_Update()
{
	_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
	_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

	for (_int i = 0; i < ColliderType_Mon::END; ++i)
	{
		if (ColliderType_Mon::RESIST == i)
		{
			m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());
			continue;
		}

		if (m_pWeaponObject != nullptr && i == ColliderType_Mon::Hand)
			continue;

		_matrix BoneMat = XMLoadFloat4x4(m_pColliderBone[i]);
		_vector vScale, vRot, vTrans;
		XMMatrixDecompose(&vScale, &vRot, &vTrans, BoneMat);

		_matrix WorldRotMat = XMMatrixRotationQuaternion(vRot);
		_matrix WorldTransMat = XMMatrixTranslationFromVector(vTrans);
		_matrix WorldMatrix = WorldRotMat * WorldTransMat * Worldmat;

		m_pColliderCom[i]->Update(WorldMatrix);
	}
}

CMonster_Normal* CMonster_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Normal* pInstance = new CMonster_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CMonster_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Normal::Clone(void* pArg)
{
	CMonster_Normal* pInstance = new CMonster_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMonster_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Normal::Free()
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
