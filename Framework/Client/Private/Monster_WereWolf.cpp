#include "pch.h"
#include "Monster_WereWolf.h"
#include "BehaviorTree_WereWolf.h"

#include "Body_WereWolf.h"
#include "Idle_WereWolf.h"

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

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-64.51f, -1.11f, 38.16f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));
	return S_OK;
}

void CMonster_WereWolf::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);
}

void CMonster_WereWolf::Update(_float fTimeDelta)
{
	State_Change();
	m_pCulStateObject->Update(this, fTimeDelta);

	Root_Move();

	m_pBodyObject->Update(fTimeDelta);
}

void CMonster_WereWolf::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);
}

HRESULT CMonster_WereWolf::Render()
{
#ifdef _DEBUG
	//for (_int i = 0; i < ENUM_CLASS(ColliderType_Mon::End); ++i)
	//{
	//	m_pColliderCom[i]->Render();
	//}
#endif
	return S_OK;
}

void CMonster_WereWolf::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

void CMonster_WereWolf::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
}

void CMonster_WereWolf::SetUp_Node(_int iTargetCellIndex, _float3 vPos)
{
}

void CMonster_WereWolf::Move_Node(_float fTimeDelta)
{
}

HRESULT CMonster_WereWolf::Ready_Components()
{
	//CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	//OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::RESIST);
	//OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::RESIST);
	//OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	//OBBDesc.vExtents = _float3(0.4f, 1.f, 0.4f);
	//OBBDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	//if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
	//	TEXT("Com_Collider_Resist"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::RESIST)]), &OBBDesc)))
	//	return E_FAIL;

	//OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	//OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_BODY);
	//OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	//OBBDesc.vExtents = _float3(0.3f, 1.1f, 0.3f);
	//OBBDesc.vCenter = _float3(0.f, -0.25f, 0.f);

	//if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
	//	TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::Body)]), &OBBDesc)))
	//	return E_FAIL;

	//OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::MON_HEAD);
	//OBBDesc.vExtents = _float3(0.12f, 0.15f, 0.12f);
	//OBBDesc.vCenter = _float3(0.f, 0.f, 0.f);

	//if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
	//	TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::Head)]), &OBBDesc)))
	//	return E_FAIL;

	//OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::MONSTER);
	//OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::ATTACK);
	//OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	//OBBDesc.vExtents = _float3(1.f, 2.f, 5.f);
	//OBBDesc.vCenter = _float3(0.f, 1.f, 1.f);

	//if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
	//	TEXT("Com_Collider_Attack"), reinterpret_cast<CComponent**>(&m_pColliderCom[ENUM_CLASS(ColliderType_Mon::ATTACK)]), &OBBDesc)))
	//	return E_FAIL;

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
	m_BlackBoard->Set_Data().iDamage = 20;

	m_BlackBoard->Set_Data().IsAttack = false;
	m_BlackBoard->Set_Data().IsChase = false;
	m_BlackBoard->Set_Data().MonPos = m_pTransformCom->Get_WorldMatrixPtr();

	m_pBehaviorTree = CBehaviorTree_WereWolf::Create(m_BlackBoard);

	return S_OK;
}

HRESULT CMonster_WereWolf::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_WereWolf::Create());

	m_pCulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_pCulStateObject);
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
