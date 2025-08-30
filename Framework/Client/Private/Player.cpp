#include "pch.h"
#include "Player.h"
#include "Client_Enum.h"
#include "Body_Player.h"
#include "Camera_Player.h"
#include "Model.h"
#include "Knife.h"
#include "HandGun.h"
#include "ShotGun.h"
#include "Sniper.h"

#include "Idle_Player.h"
#include "Guard_Player.h"
#include "Aim_Player.h"
#include "Reload_Player.h"
#include "Attack_Player.h"
#include "Player_Manager.h"
#include "WeaponSwap_Player.h"
#include "Move_Player.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& Prototype) : CContainerObject (Prototype)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{

	m_iCulWeponState = ENUM_CLASS(PLAYER_WEAPON::NONE);
	m_szAnimTag = "Idle_Loop";
	m_szCulStateTag = TEXT("Idle");
	m_szPreStateTag = m_szCulStateTag;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;

	m_pColliderBone[BODY] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[PLAYER_VIEW] = m_pBodyObject->Get_BoneMatrix(TEXT("Cam"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-61.f, -8.5f, 13.5f, 1.f));
	CPlayer_Manager::GetInstance()->Update_Cell(m_pNavigationCom->Get_CulIndex());
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->PrePostion_Update();
	m_pCamera->Priority_Update(fTimeDelta);
	m_MoveState = {};
	m_AttackState = {};

	m_pBodyObject->Priority_Update(fTimeDelta);
	m_pWeaponObject->Priority_Update(fTimeDelta);
	if (m_pGameInstance->IsKeyDown(DIK_5))
		m_bisCameraLock ? m_bisCameraLock = false : m_bisCameraLock = true;

}

void CPlayer::Update(_float fTimeDelta)
 {	  
	if (!InputKey_UI() && !m_bisCameraLock)
	{
		InputKey_MoveState(fTimeDelta);
		InputKey_AttackState(fTimeDelta);
		InputKey_WeaponChange(fTimeDelta);
		m_fYaw += m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X) * 0.1f * fTimeDelta;
		Rotaion_Upper(fTimeDelta);
		m_pTransformCom->Rotation_All(_float3{ 0.f, m_fYaw, 0.f });
	}
	
	m_CulStateObject->Update(this, fTimeDelta);
	if (m_szPreStateTag != m_szCulStateTag)
	{
		m_CulStateObject->Exit(this);
		Safe_Release(m_CulStateObject);

		m_CulStateObject = Find_StateObject(m_szCulStateTag);
		Safe_AddRef(m_CulStateObject);

		m_CulStateObject->Enter(this);
		m_szPreStateTag = m_szCulStateTag;
	}

	m_bIsAnimFinsh = false;

	m_pBodyObject->Update(fTimeDelta);
	m_pWeaponObject->Update(fTimeDelta);
	
	m_pCamera->Update(fTimeDelta);

	IsDmage(fTimeDelta);

	m_pTransformCom->Set_State(Engine::STATE::POSITION,	m_pNavigationCom->Compute_OnCell(m_pTransformCom->Get_State(Engine::STATE::POSITION)));
	CPlayer_Manager::GetInstance()->Set_PlayerPos(m_pTransformCom->Get_State(STATE::POSITION));
	Collider_Update();

}
	
void CPlayer::Late_Update(_float fTimeDelta)
{
	for (_int i = 0; i < ColliderType_Player::END; ++i)
	{
		if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom[i])))
			return;
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);

	m_pWeaponObject->Late_Update(fTimeDelta);
	m_pCamera->Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	for (_int i = 0; i < ColliderType_Player::END; ++i)
	{
		m_pColliderCom[i]->Render();
	}
	
#endif
	return S_OK;
}

void CPlayer::Switch_Anim(string szAnimTag, _bool IsLoop)
{
	m_szAnimTag = szAnimTag;
	m_bIsAnimLoop = IsLoop;
}

void CPlayer::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
	if (MyCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
	{
		if (TargetCollision.iObjType == ENUM_CLASS(OBJECT_TYPE::RESIST))
			m_pTransformCom->Is_Sliding(m_pNavigationCom, XMLoadFloat3(&TargetCollision.pCollider->Get_Intersect_Normal()));
	}

	switch (TargetCollision.iObjType)
	{
	case ENUM_CLASS(OBJECT_TYPE::WEAPON):
		if (!m_bIsDamage)
		{
			m_bIsDamage = true;
			m_fDamageCool = 3.f;
		}
		break;
	}
}

HRESULT CPlayer::Ready_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::RESIST);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::RESIST);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.4f, 0.4f, 0.4f);
	OBBDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Resist"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Player::RESIST]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::PLAYER_VIEW);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::PLAYER_VIEW);
	OBBDesc.vAngles = _float3(0.f, 0.1f, 0.f);
	OBBDesc.vExtents = _float3(0.1f, 0.1f, 1.f);
	OBBDesc.vCenter = _float3(-0.092f, 0.0f, -1.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_View"), reinterpret_cast<CComponent**>(&m_pColliderCom[ColliderType_Player::PLAYER_VIEW]), &OBBDesc)))
		return E_FAIL;


	OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::PLAYER);
	OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::PLAYER);
	OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.vExtents = _float3(0.2f, 0.67f, 0.2f);
	OBBDesc.vCenter = _float3(0.f, -0.15f, 0.f);

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC        NaviDesc{};
	NaviDesc.iCurrentCellIndex = 0;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_DESC BodyDesc{};
	BodyDesc.pWeaponState = &m_iCulWeponState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pAnimTag = &m_szAnimTag;
	BodyDesc.pIsAnimLoop = &m_bIsAnimLoop;
	BodyDesc.pIsAnimFinsh = &m_bIsAnimFinsh;

	if(FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;
	
	CBody_Player* pBody = static_cast<CBody_Player*>(Find_PartObject(TEXT("Part_Body")));
	if (pBody == nullptr)
		return E_FAIL;
	m_pBodyObject = pBody;  
	Safe_AddRef(m_pBodyObject);

	CWeaponObject::WEAPON_DESC WeaponDesc{};
	WeaponDesc.pCulStateTag = &m_szCulStateTag;
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_MiddleF1"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_Knife"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Knife_Player"), &WeaponDesc)))
		return E_FAIL;

	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_Wep"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_HandGun"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HandGun_Player"), &WeaponDesc)))
		return E_FAIL;

	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_Wep"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_ShotGun"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ShotGun_Player"), &WeaponDesc)))
		return E_FAIL;

	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_Wep"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_Sniper"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sniper_Player"), &WeaponDesc)))
		return E_FAIL;

	m_pWeaponObject = Find_PartObject(TEXT("Part_Knife"));

	CCamera_Player::CAMERA_PLAYER_DESC CameraDesc{};
	CameraDesc.vEye = _float4(0.f, 0.f, 0.1f, 1.f);
	CameraDesc.vAt = _float4(-0.1f, 0.f, -1.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(45.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fMouseSensor = 0.2f;
	CameraDesc.pSocketMatrix1 = pBody->Get_BoneMatrix(TEXT("Cam"));
	CameraDesc.pSocketMatrix2 = pBody->Get_BoneMatrix(TEXT("CamAdd_A"));
	CameraDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	m_pCamera = dynamic_cast<CCamera_Player*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc));

	return S_OK;
}

HRESULT CPlayer::Ready_StateObjects()
{
	Add_StateObject(TEXT("Idle"), CIdle_Player::Create());
	Add_StateObject(TEXT("Guard"), CGuard_Player::Create());
	Add_StateObject(TEXT("Aim"), CAim_Player::Create());
	Add_StateObject(TEXT("Reload"), CReload_Player::Create());
	Add_StateObject(TEXT("Attack"), CAttack_Player::Create());
	Add_StateObject(TEXT("WeaponSwap"), CWeaponSwap_Player::Create());
	Add_StateObject(TEXT("Move"), CMove_Player::Create());

	m_CulStateObject = Find_StateObject(TEXT("Idle"));
	Safe_AddRef(m_CulStateObject);
	m_CulStateObject->Enter(this);
	return S_OK;
}

HRESULT CPlayer::Add_StateObject(const _wstring& strStateObjectTag, CPlayerState* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CPlayerState* CPlayer::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CPlayer::InputKey_MoveState(_float fTimeDelta)
{
	_bool IsMove = true;

	//이동 상태 값
	_vector vPos =	m_pTransformCom->Get_State(STATE::POSITION);
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT))
	{
		m_MoveState.isJog = true;
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_MoveState.isMoveF = true;
		m_pTransformCom->Go_Straight(fTimeDelta);
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_MoveState.isMoveB = true;
		m_pTransformCom->Go_Backward(fTimeDelta);
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_MoveState.isMoveL = true;
		m_pTransformCom->Go_Left(fTimeDelta);
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_MoveState.isMoveR = true;
		m_pTransformCom->Go_Right(fTimeDelta);
		IsMove = true;
	}

	if (IsMove)
	{
		m_AttackState.isMove = true;
	}
	m_pTransformCom->Is_Sliding(m_pNavigationCom);
	_int Index = m_pNavigationCom->Get_CulIndex();
	if (Index == 7)
		int a = 10;
	CPlayer_Manager::GetInstance()->Update_Cell(Index);
}

void CPlayer::InputKey_AttackState(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyHold(DIK_Q))
	{
		m_AttackState.isGuard = true;
	}
	else if (m_iCulWeponState != ENUM_CLASS(PLAYER_WEAPON::KNIFE) && m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::RB))
	{
		m_AttackState.isAim = true;
	}

	if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::LB))
	{
		m_AttackState.isAttack = true;
		RAY_DESC RayDesc = m_pGameInstance->Create_FpsRayDesc(0, 0);
		m_pGameInstance->Add_ColliderRay(ENUM_CLASS(COLLISION_LAYER::RAY), ENUM_CLASS(OBJECT_TYPE::RAY), RayDesc);
	}

	if (m_pGameInstance->IsKeyHold(DIK_R))
	{
		m_AttackState.isReload = true;
	}

}

void CPlayer::InputKey_WeaponChange(_float fTimeDelta)
{
	if (m_szCulStateTag == TEXT("Attack") || m_szCulStateTag == TEXT("Reload") || m_szCulStateTag == TEXT("Aim") || m_szCulStateTag == TEXT("Guard"))
		return;

	_int iSlotIndex = { 0 };
	_int iSlotItem = {-1};
	_bool bIsSelete = { false };
	
	if (bIsSelete = m_pGameInstance->IsKeyDown(DIK_1))
		iSlotIndex = 1;
	else if (bIsSelete = m_pGameInstance->IsKeyDown(DIK_2))
		iSlotIndex = 2;
	else if (bIsSelete = m_pGameInstance->IsKeyDown(DIK_3))
		iSlotIndex = 3;
	else if (bIsSelete = m_pGameInstance->IsKeyDown(DIK_4))
		iSlotIndex = 4;

	if (bIsSelete)
	{
		m_pGameInstance->Publish(Event_QUICK_UI_OPEN{});
		CPlayer_Manager::GetInstance()->Selete_Slot(iSlotIndex);
		iSlotItem =	CPlayer_Manager::GetInstance()->Get_QuickSlotItem(iSlotIndex);

		if (iSlotItem == 0 && m_iCulWeponState != ENUM_CLASS(PLAYER_WEAPON::HANDGUN))
		{
			m_iNextWeponState = ENUM_CLASS(PLAYER_WEAPON::HANDGUN);
			m_AttackState.isWeaponSwap = true;
		}
		else if (iSlotItem == 1 && m_iCulWeponState != ENUM_CLASS(PLAYER_WEAPON::SHOTGUN))
		{
			m_iNextWeponState = ENUM_CLASS(PLAYER_WEAPON::SHOTGUN);
			m_AttackState.isWeaponSwap = true;
		}
		else if (iSlotItem == 2 && m_iCulWeponState != ENUM_CLASS(PLAYER_WEAPON::SNIPER))
		{
			m_iNextWeponState = ENUM_CLASS(PLAYER_WEAPON::SNIPER);
			m_AttackState.isWeaponSwap = true;
		}
		else if (iSlotItem == 3 && m_iCulWeponState != ENUM_CLASS(PLAYER_WEAPON::KNIFE))
		{
			m_iNextWeponState = ENUM_CLASS(PLAYER_WEAPON::KNIFE);
			m_AttackState.isWeaponSwap = true;
		}
		else if(m_iCulWeponState != ENUM_CLASS(PLAYER_WEAPON::NONE))
		{
			m_iNextWeponState = ENUM_CLASS(PLAYER_WEAPON::NONE);
			m_AttackState.isWeaponSwap = true;
		}
	}

	if (m_iPreWeponState != m_iCulWeponState)
	{
		switch (m_iCulWeponState)
		{
		case ENUM_CLASS(PLAYER_WEAPON::KNIFE):
			m_pWeaponObject = Find_PartObject(TEXT("Part_Knife"));
			CPlayer_Manager::GetInstance()->Set_Damage(10);
			break;
		case ENUM_CLASS(PLAYER_WEAPON::HANDGUN):
			m_pWeaponObject = Find_PartObject(TEXT("Part_HandGun"));
			CPlayer_Manager::GetInstance()->Set_Damage(20);
			break;
		case ENUM_CLASS(PLAYER_WEAPON::SHOTGUN):
			m_pWeaponObject = Find_PartObject(TEXT("Part_ShotGun"));
			CPlayer_Manager::GetInstance()->Set_Damage(50);
			break;
		case ENUM_CLASS(PLAYER_WEAPON::SNIPER):
			m_pWeaponObject = Find_PartObject(TEXT("Part_Sniper"));
			CPlayer_Manager::GetInstance()->Set_Damage(100);
			break;
		}
		m_iPreWeponState = m_iCulWeponState;
	}

}

_bool CPlayer::InputKey_UI()
{

	if (m_szCulStateTag == TEXT("Attack") || m_szCulStateTag == TEXT("Reload") || m_szCulStateTag == TEXT("Aim") || m_szCulStateTag == TEXT("Guard"))
		return false;

	if (m_pGameInstance->IsKeyDown(DIK_TAB) && !m_bIsUIOpen)
	{
		m_pGameInstance->Publish(Event_Inventory_Open{ {true} });
		m_bIsUIOpen = true;
	}
	else if (m_pGameInstance->IsKeyDown(DIK_TAB) && m_bIsUIOpen)
	{
		m_pGameInstance->Publish(Event_Inventory_Open{ {false} });
		m_bIsUIOpen = false;
	}

	
	return m_bIsUIOpen;
}

void CPlayer::Rotaion_Upper(_float fTimeDelta)
{
	m_fPitch -= _float(m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y) * 0.1 * fTimeDelta);
	
	if (m_fPitch >= XMConvertToRadians(45.f))
		m_fPitch = XMConvertToRadians(45.f);
	else if (m_fPitch <= XMConvertToRadians(-45.f))
		m_fPitch = XMConvertToRadians(-45.f);

	m_pBodyObject->Model_Upper_Rot(m_fPitch);

}

void CPlayer::Collider_Update()
{
	{
		_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
		_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

		_matrix BoneMat = XMLoadFloat4x4(m_pColliderBone[BODY]);
		_vector vScale, vRot, vTrans;
		XMMatrixDecompose(&vScale, &vRot, &vTrans, BoneMat);

		_matrix WorldRotMat = XMMatrixRotationQuaternion(vRot);
		_matrix WorldTransMat = XMMatrixTranslationFromVector(vTrans);
		_matrix WorldMatrix = WorldRotMat * WorldTransMat * Worldmat;

		m_pColliderCom[ColliderType_Player::BODY]->Update(WorldMatrix);
	}

	{
		_matrix matPitch = XMMatrixRotationX(m_fPitch);
		_matrix Worldmat = m_pTransformCom->Get_WorldMatrix();
		_vector vRotation = XMQuaternionRotationMatrix(Worldmat);

		_matrix BoneMat = XMLoadFloat4x4(m_pColliderBone[PLAYER_VIEW]);
		_vector vScale, vRot, vTrans;
		XMMatrixDecompose(&vScale, &vRot, &vTrans, BoneMat);

		_matrix WorldRotMat = XMMatrixRotationQuaternion(vRot);
		_matrix WorldTransMat = XMMatrixTranslationFromVector(vTrans);
		_matrix WorldMatrix = WorldRotMat * WorldTransMat * Worldmat;

		m_pColliderCom[ColliderType_Player::PLAYER_VIEW]->Update(WorldMatrix);
	}
	m_pColliderCom[ColliderType_Player::RESIST]->Update(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer::IsDmage(_float fTimeDelta)
{
	if (m_bIsDamage)
	{
		m_fDamageCool -= fTimeDelta;

		if (m_fDamageCool <= 0.f)
		{
			m_bIsDamage = false;
			CPlayer_Manager::GetInstance()->Damage_On();
		}
	}
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pCamera);

	Safe_Release(m_pBodyObject);
	for (auto& Pair : m_StateObjects)
		Safe_Release(Pair.second);

	m_StateObjects.clear();
	Safe_Release(m_CulStateObject);
	Safe_Release(m_pNavigationCom);

	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);

}
