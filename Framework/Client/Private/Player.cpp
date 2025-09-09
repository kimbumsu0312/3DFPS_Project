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

	m_iAnimState = ENUM_CLASS(PLAYER_ANIM::NONE);
	m_szAnimTag = "Walk_Loop";
	m_szCulStateTag = TEXT("Idle");
	m_szPreStateTag = m_szCulStateTag;

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

	m_pColliderBone[BODY] = m_pBodyObject->Get_BoneMatrix(TEXT("Spine_0"));
	m_pColliderBone[PLAYER_VIEW] = m_pBodyObject->Get_BoneMatrix(TEXT("Cam"));

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-30.95f, -8.97f, 63.67f, 1.f));
	CPlayer_Manager::GetInstance()->Update_Cell(m_pNavigationCom->Get_CulIndex());
	m_pTransformCom->Rotation(_vector{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(180.f));

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->PrePostion_Update();
	m_pCamera->Priority_Update(fTimeDelta);

	m_pBodyObject->Priority_Update(fTimeDelta);
	if (m_pWeaponObject != nullptr)
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
	State_Check();
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
	if(m_pWeaponObject != nullptr)
		m_pWeaponObject->Update(fTimeDelta);
	
	m_pCamera->Update(fTimeDelta);

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

	_float4 vPlayerPos{};

	XMStoreFloat4(&vPlayerPos, XMVectorSetW(m_pTransformCom->Get_State(STATE::POSITION), 1.f));
	m_pGameInstance->Update_LightPotion(TEXT("Light_Player"), vPlayerPos);
	m_pBodyObject->Late_Update(fTimeDelta);
	if (m_pWeaponObject != nullptr)
		m_pWeaponObject->Late_Update(fTimeDelta);
	
	m_pCamera->Late_Update(fTimeDelta);

#ifdef _DEBUG
	for (_int i = 0; i < ColliderType_Player::END; ++i)
	{
		m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
	}

#endif
}

HRESULT CPlayer::Render()
{
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
		break;
	}
}

void CPlayer::WeaponSwap()
{
	m_iAnimState = m_iNextAnim;
	m_BlackBoard->Set_Data().isWeaponSwap = false;
	Safe_Release(m_pWeaponObject);
	switch (m_iAnimState)
	{
	case ENUM_CLASS(PLAYER_ANIM::KNIFE):
		m_iRayCount = 0;
		m_pWeaponObject = Find_PartObject(TEXT("Part_Knife"));
		Safe_AddRef(m_pWeaponObject);
		m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::END });
		CPlayer_Manager::GetInstance()->Set_Damage(20);
		break;
	case ENUM_CLASS(PLAYER_ANIM::HANDGUN):
		m_iRayCount = 1;
		m_pWeaponObject = Find_PartObject(TEXT("Part_HandGun"));
		Safe_AddRef(m_pWeaponObject);
		m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::PISTOL });
		CPlayer_Manager::GetInstance()->Set_Damage(10);
		break;
	case ENUM_CLASS(PLAYER_ANIM::SHOTGUN):
		m_iRayCount = 8;
		m_pWeaponObject = Find_PartObject(TEXT("Part_ShotGun"));
		Safe_AddRef(m_pWeaponObject);
		m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::SHOTGUN });
		CPlayer_Manager::GetInstance()->Set_Damage(5);
		break;
	case ENUM_CLASS(PLAYER_ANIM::SNIPER):
		m_iRayCount = 1;
		m_pWeaponObject = Find_PartObject(TEXT("Part_Sniper"));
		Safe_AddRef(m_pWeaponObject);
		m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::SNIPER });
		CPlayer_Manager::GetInstance()->Set_Damage(100);
		break;
	case ENUM_CLASS(PLAYER_ANIM::END):
		m_iRayCount = 0;
		m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::END });
		CPlayer_Manager::GetInstance()->Set_Damage(0);
		break;
	}

}

void CPlayer::AttackCollider()
{
	if (m_iAnimState == ENUM_CLASS(PLAYER_ANIM::KNIFE))
		static_cast<CWeaponObject*>(m_pWeaponObject)->Add_Collider();
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
	OBBDesc.vExtents = _float3(0.05f, 0.05f, 1.f);
	OBBDesc.vCenter = _float3(-0.1f, 0.0f, -1.f);

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
	NaviDesc.iCurrentCellIndex = 7557;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_DESC BodyDesc{};
	BodyDesc.m_BlackBoard = m_BlackBoard;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

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

HRESULT CPlayer::Ready_Utility()
{
	m_BlackBoard = CBlackBoard<PLAYER_DATA>::Create();
	m_BlackBoard->Set_Data().bIsAnimFinsh = &m_bIsAnimFinsh;
	m_BlackBoard->Set_Data().bIsAnimLoop = &m_bIsAnimLoop;
	m_BlackBoard->Set_Data().iAnimState = &m_iAnimState;
	m_BlackBoard->Set_Data().szAnimTag = &m_szAnimTag;
	m_BlackBoard->Set_Data().szCulStateTag = &m_szCulStateTag;

	m_BlackBoard->Set_Data().bIsDamage = false;
	m_BlackBoard->Set_Data().fDamageCool = 0.f;
	m_BlackBoard->Set_Data().isJog = false;
	m_BlackBoard->Set_Data().isMove = false;
	m_BlackBoard->Set_Data().isAttack = false;
	m_BlackBoard->Set_Data().isGuard = false;
	m_BlackBoard->Set_Data().isReload = false;
	m_BlackBoard->Set_Data().isAim = false;
	m_BlackBoard->Set_Data().isWeaponSwap = false;

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
	_bool IsMove = false;
	_bool IsJog = false;

	//이동 상태 값
	_vector vPos =	m_pTransformCom->Get_State(STATE::POSITION);
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT))
	{
		IsJog = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		IsJog ? m_pTransformCom->Go_Straight(fTimeDelta * 1.5f) : m_pTransformCom->Go_Straight(fTimeDelta);
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		IsJog ? m_pTransformCom->Go_Backward(fTimeDelta * 1.5f) : m_pTransformCom->Go_Backward(fTimeDelta);
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		IsJog ? m_pTransformCom->Go_Left(fTimeDelta * 1.5f) : m_pTransformCom->Go_Left(fTimeDelta);
		IsMove = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		IsJog ? m_pTransformCom->Go_Right(fTimeDelta * 1.5f) : m_pTransformCom->Go_Right(fTimeDelta);
		IsMove = true;
	}

	m_BlackBoard->Set_Data().isMove = IsMove;
	m_BlackBoard->Set_Data().isJog = IsJog;

	m_pTransformCom->Is_Sliding(m_pNavigationCom);
	_int Index = m_pNavigationCom->Get_CulIndex();

	CPlayer_Manager::GetInstance()->Update_Cell(Index);
}

void CPlayer::InputKey_AttackState(_float fTimeDelta)
{
	if (*m_BlackBoard->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::NONE))
		return;

	m_BlackBoard->Set_Data().isGuard = false;
	if (m_pGameInstance->IsKeyHold(DIK_Q))
		m_BlackBoard->Set_Data().isGuard = true;
	
	m_BlackBoard->Set_Data().isAim = false;
	if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::RB) && *m_BlackBoard->Get_Data().iAnimState != ENUM_CLASS(PLAYER_ANIM::KNIFE))
		m_BlackBoard->Set_Data().isAim = true;

	if (m_pGameInstance->IsKeyDown(DIK_R) && m_iAnimState != ENUM_CLASS(PLAYER_ANIM::KNIFE) && m_BlackBoard->Get_Data().isWeaponSwap == false)
	{
		if (m_BlackBoard->Get_Data().isReload == false)
		{
			if (CPlayer_Manager::GetInstance()->Reload())
				m_BlackBoard->Set_Data().isReload = true;
		}
	}
	if (m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::LB))
	{
		
		if (m_iAnimState != ENUM_CLASS(PLAYER_ANIM::KNIFE))
		{
			if (CPlayer_Manager::GetInstance()->Gun_Shoting())
			{
				m_BlackBoard->Set_Data().isAttack = true;

				for (_int i = 0; i < m_iRayCount; ++i)
				{
					RAY_DESC RayDesc = m_pGameInstance->Create_FpsRayDesc(m_fRayRange);
					m_pGameInstance->Add_ColliderRay(ENUM_CLASS(COLLISION_LAYER::RAY), ENUM_CLASS(OBJECT_TYPE::RAY), RayDesc);
				}
			}
		}
		else
			m_BlackBoard->Set_Data().isAttack = true;
	}

	switch (m_iAnimState)
	{
	case ENUM_CLASS(PLAYER_ANIM::SHOTGUN):
		m_BlackBoard->Get_Data().isAim ? m_fRayRange = 50 : m_fRayRange = 120;
		break;
	case ENUM_CLASS(PLAYER_ANIM::HANDGUN):
		m_BlackBoard->Get_Data().isAim ? m_fRayRange = 0 : m_fRayRange = 100;
		break;
	case ENUM_CLASS(PLAYER_ANIM::SNIPER):
		m_BlackBoard->Get_Data().isAim ? m_fRayRange = 0 : m_fRayRange = 50;
		break;
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

		if (iSlotItem == 0 && m_iAnimState != ENUM_CLASS(PLAYER_ANIM::HANDGUN))
		{
			m_iNextAnim = ENUM_CLASS(PLAYER_ANIM::HANDGUN);
			m_BlackBoard->Set_Data().isWeaponSwap = true;
		}
		else if (iSlotItem == 1 && m_iAnimState != ENUM_CLASS(PLAYER_ANIM::SHOTGUN))
		{
			m_iNextAnim = ENUM_CLASS(PLAYER_ANIM::SHOTGUN);
			m_BlackBoard->Set_Data().isWeaponSwap = true;
		}
		else if (iSlotItem == 2 && m_iAnimState != ENUM_CLASS(PLAYER_ANIM::SNIPER))
		{
			m_iNextAnim = ENUM_CLASS(PLAYER_ANIM::SNIPER);
			m_BlackBoard->Set_Data().isWeaponSwap = true;
		}
		else if (iSlotItem == 3 && m_iAnimState != ENUM_CLASS(PLAYER_ANIM::KNIFE))
		{
			m_iNextAnim = ENUM_CLASS(PLAYER_ANIM::KNIFE);
			m_BlackBoard->Set_Data().isWeaponSwap = true;
		}
		else if (m_iAnimState != ENUM_CLASS(PLAYER_ANIM::NONE))
		{
			m_iNextAnim = ENUM_CLASS(PLAYER_ANIM::NONE);
			m_BlackBoard->Set_Data().isWeaponSwap = false;
		}
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

void CPlayer::State_Check()
{
	if (m_BlackBoard->Get_Data().isReload == true)
		m_szCulStateTag = TEXT("Reload");
	else if (m_BlackBoard->Get_Data().isWeaponSwap == true)
		m_szCulStateTag = TEXT("WeaponSwap");
	else if (m_BlackBoard->Get_Data().isGuard == true)
		m_szCulStateTag = TEXT("Guard");
	else if (m_BlackBoard->Get_Data().isAttack == true)
		m_szCulStateTag = TEXT("Attack");
	else if (m_BlackBoard->Get_Data().isAim == true)
		m_szCulStateTag = TEXT("Aim");
	else if (m_BlackBoard->Get_Data().isMove == true)
		m_szCulStateTag = TEXT("Move");
	else
		m_szCulStateTag = TEXT("Idle");
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

	Safe_Release(m_pWeaponObject);
	Safe_Release(m_BlackBoard);
	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);

}
