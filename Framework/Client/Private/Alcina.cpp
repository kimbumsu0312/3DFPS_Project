#include "pch.h"
#include "Alcina.h"
#include "Player_Manager.h"
#include "Body_Alcina.h"
#include "BehaviorTree_Alcina.h"
#include "BlackBoard.h"
#include "MonState_Alcina.h"

#include "Walk_Alcina.h"
#include "Idle_Alcina.h"
#include "Attack_Alcina.h"
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
	m_iHp = 10000;
	m_iAnimState = ENUM_CLASS(BOSS_SISTER::NORMAL);
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


	m_pColliderBone[ColliderType_Mon::Body] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[ColliderType_Mon::Head] = m_pBodyObject->Get_BoneMatrix(TEXT("Head"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-55.66f, -8.68f, 30.52f, 1.f));
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));
	return S_OK;
}

void CAlcina::Priority_Update(_float fTimeDelta)
{
	m_bIsHeadShot = false;
	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);
}

void CAlcina::Update(_float fTimeDelta)
{
	m_pBehaviorTree->Update();
	State_Change();
	m_pCulStateObject->Update(this, fTimeDelta);
	
	m_pBodyObject->Update(fTimeDelta);

	//위치 보정
	Root_Move();
	m_pTransformCom->Set_State(Engine::STATE::POSITION,
		m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));

	Collider_Update();
}

void CAlcina::Late_Update(_float fTimeDelta)
{
	for (_int i = 0; i < ColliderType_Mon::End; ++i)
	{
		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
			return;
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);
}

HRESULT CAlcina::Render()
{
#ifdef _DEBUG
	for (_int i = 0; i < ColliderType_Mon::End; ++i)
	{
		m_pColliderCom[i]->Render();
	}

#endif
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

void CAlcina::IsDamage()
{
}

void CAlcina::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
	{
		if (TargetCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
			m_pTransformCom->Is_Sliding(m_pNavigationCom, XMLoadFloat3(&TargetCollision.pCollider->Get_Intersect_Normal()));
	}
	else if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::ATTACK))
	{
		if (TargetCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::PLAYER))
			m_bIsAttack = true;
	}
	else
	{
		switch (TargetCollision.iObjType)
		{
		case ENUM_CLASS(OBJECT_TYPE::RAY):
			if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::MON_HEAD))
				m_bIsHeadShot = true;
			m_iDamage += CPlayer_Manager::GetInstance()->Get_Damage();
			break;
		}
	}

}

void CAlcina::SetUp_Node(_int iTargetCellIndex, _float3 vPos)
{
	m_pNavigationCom->SetUp_Node(iTargetCellIndex, vPos);
}

void CAlcina::Move_Node(_float fTimeDelta)
{
	_vector vMonPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vPlayerPos = m_pNavigationCom->IsNaviNode(vMonPos);

	_vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos - vMonPos, 0.f));
	_vector vLook = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(STATE::LOOK), 0.f));

	_vector vAxis = XMVector3Normalize(XMVector3Cross(vLook, vDir));

	m_pTransformCom->Turn(vAxis, fTimeDelta);

	m_pTransformCom->Go_Straight(fTimeDelta);
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
	OBBDesc.vExtents = _float3(1.f, 2.f, 5.f);
	OBBDesc.vCenter = _float3(0.f, 1.f, 1.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Attack"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Mon::ATTACK]), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 1265;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
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
	m_BlackBoard->Set_Data().iHp = &m_iHp;
	m_BlackBoard->Set_Data().iDamage = &m_iDamage;
	m_BlackBoard->Set_Data().IsAttack = &m_bIsAttack;

	m_BlackBoard->Set_Data().IsChase = &m_bIsChase;
	m_BlackBoard->Set_Data().MonPos = m_pTransformCom->Get_WorldMatrixPtr();

	m_BlackBoard->Set_Data().eAttackType = Attack_Type::END;
	m_pBehaviorTree = CBehaviorTree_Alcina::Create(m_BlackBoard);

	return S_OK;
}

HRESULT CAlcina::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_Alcina::Create());
	Add_StateObject(TEXT("Attack"), CAttack_Alcina::Create());
	Add_StateObject(TEXT("Walk"), CWalk_Alcina::Create());
	Add_StateObject(TEXT("Damage"), CDamage_Alcina::Create());
	Add_StateObject(TEXT("Die"), CDie_Alcina::Create());

	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
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
	//월드 분해
	_vector vScale, vWorldRot, vWorldTrans;
	XMMatrixDecompose(&vScale, &vWorldRot, &vWorldTrans, m_pTransformCom->Get_WorldMatrix());
	_vector vMovePos = XMLoadFloat3(m_pBodyObject->Get_MovePos());
	_vector vMoveRot = XMLoadFloat4(m_pBodyObject->Get_MoveRot());

	//회전량 누적
	vWorldRot = XMQuaternionMultiply(vMoveRot, vWorldRot);
	//vWorldRot = XMQuaternionNormalize(vWorldRot);
	
	//월드 기준으로 방향 보정
	if(m_iAnimState == ENUM_CLASS(ANIM_STATE::DAMAGE))
		vMovePos = XMVector3Rotate(vMovePos, XMQuaternionIdentity());
	else
		vMovePos = XMVector3Rotate(vMovePos, vWorldRot);
	//이동량 누적
	vWorldTrans += vMovePos;

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
		if (ColliderType_Mon::RESIST == i || ColliderType_Mon::ATTACK == i)
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
}
