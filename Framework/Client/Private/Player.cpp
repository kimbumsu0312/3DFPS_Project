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
#include "Walk_Player.h"
#include "Guard_Player.h"
#include "Aim_Player.h"
#include "Reload_Player.h"
#include "Attack_Player.h"
#include "Player_Manager.h"

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
	m_iCulWeponState = PLAYER_WEAPON::KNIFE;
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

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 3.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	m_pCamera->Priority_Update(fTimeDelta);
	m_MoveState = {};
	m_AttackState = {};

	m_pBodyObject->Priority_Update(fTimeDelta);
	m_pWeaponObject->Priority_Update(fTimeDelta);
	if (m_pGameInstance->IsKeyDown(DIK_5))
		m_bisCameraLock ? m_bisCameraLock = false : m_bisCameraLock = true;

	if(!m_bisCameraLock)
		Rotaion_Upper(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
 {
	if (!InputKey_UI() && !m_bisCameraLock)
	{
		InputKey_MoveState(fTimeDelta);
		InputKey_AttackState(fTimeDelta);
		InputKey_WeaponChange(fTimeDelta);
	}
 	m_CulStateObject->Update(fTimeDelta, m_AttackState, m_MoveState);
	if (m_szPreStateTag != m_szCulStateTag)
	{
		m_CulStateObject->Exit();
		Safe_Release(m_CulStateObject);

		m_CulStateObject = Find_StateObject(m_szCulStateTag);
		Safe_AddRef(m_CulStateObject);

		m_CulStateObject->Enter(m_AttackState, m_MoveState);
		m_szPreStateTag = m_szCulStateTag;
	}

	m_bIsAnimFinsh = false;
	m_pBodyObject->Update(fTimeDelta);
	m_pWeaponObject->Update(fTimeDelta);
	m_pCamera->Update(fTimeDelta);
	if (!m_bisCameraLock)
	{
		m_fYaw += m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X) * 0.1f * fTimeDelta;
		m_pTransformCom->Rotation_All(_float3{ 0.f, m_fYaw, 0.f });
	}
	CPlayer_Manager::GetInstance()->Set_PlayerPos(m_pTransformCom->Get_State(STATE::POSITION));
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pBodyObject->Late_Update(fTimeDelta);

	m_pWeaponObject->Late_Update(fTimeDelta);
	m_pCamera->Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return S_OK;
}



HRESULT CPlayer::Ready_Components()
{

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
	CPlayerState::Player_STATE_DESC Desc{};
	Desc.pWeaponState = &m_iCulWeponState;
	Desc.pAnimTag = &m_szAnimTag;
	Desc.pStateTag = &m_szCulStateTag;
	Desc.pIsAnimLoop = &m_bIsAnimLoop;
	Desc.pIsAnimFinsh = &m_bIsAnimFinsh;
	CIdle_Player* pInstance = CIdle_Player::Create(&Desc);
	if (pInstance == nullptr)
		return E_FAIL;

	Add_StateObject(TEXT("Idle"), pInstance);
	m_CulStateObject = pInstance;
	m_CulStateObject->Enter();
	Safe_AddRef(pInstance);


	Add_StateObject(TEXT("Guard"), CGuard_Player::Create(&Desc));
	//Add_StateObject(TEXT("Walk"), CWalk_Player::Create(&Desc));
	Add_StateObject(TEXT("Aim"), CAim_Player::Create(&Desc));
	Add_StateObject(TEXT("Reload"), CReload_Player::Create(&Desc));
	Add_StateObject(TEXT("Attack"), CAttack_Player::Create(&Desc));
	return S_OK;
}

HRESULT CPlayer::Add_StateObject(const _wstring& strStateObjectTag, CStateObject* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CStateObject* CPlayer::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CPlayer::InputKey_MoveState(_float fTimeDelta)
{
	//이동 상태 값
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT))
	{
		m_MoveState.isJog = true;
	}
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_MoveState.isMoveF = true;
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_MoveState.isMoveB = true;
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_MoveState.isMoveL = true;
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_MoveState.isMoveR = true;
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

void CPlayer::InputKey_AttackState(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyHold(DIK_Q))
	{
		m_AttackState.isGuard = true;
	}
	else if (m_iCulWeponState != PLAYER_WEAPON::KNIFE && m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::RB))
	{
		m_AttackState.isAim = true;
	}

	if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::LB))
	{
		m_AttackState.isAttack = true;
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

	if (m_pGameInstance->IsKeyDown(DIK_1))
		m_iCulWeponState = PLAYER_WEAPON::KNIFE;
	if (m_pGameInstance->IsKeyDown(DIK_2))
		m_iCulWeponState = PLAYER_WEAPON::SHOTGUN;
	if (m_pGameInstance->IsKeyDown(DIK_3))
		m_iCulWeponState = PLAYER_WEAPON::SNIPER;
	if (m_pGameInstance->IsKeyDown(DIK_4))
		m_iCulWeponState = PLAYER_WEAPON::HANDGUN;

	if (m_iPreWeponState != m_iCulWeponState)
	{
		switch (m_iCulWeponState)
		{
		case PLAYER_WEAPON::KNIFE:
			m_pWeaponObject = Find_PartObject(TEXT("Part_Knife"));

			break;
		case PLAYER_WEAPON::HANDGUN:
			m_pWeaponObject = Find_PartObject(TEXT("Part_HandGun"));
			m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::PISTOL });
			break;
		case PLAYER_WEAPON::SHOTGUN:
			m_pWeaponObject = Find_PartObject(TEXT("Part_ShotGun"));
			m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::SHOTGUN });
			break;
		case PLAYER_WEAPON::SNIPER:
			m_pWeaponObject = Find_PartObject(TEXT("Part_Sniper"));
			m_pGameInstance->Publish(Event_Weapon_Selete{ WEAPON_TYPE::SNIPER });
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
 	m_fPitch -= m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y) * 0.1 * fTimeDelta;
	
	if (m_fPitch >= XMConvertToRadians(45.f))
		m_fPitch = XMConvertToRadians(45.f);
	else if (m_fPitch <= XMConvertToRadians(-45.f))
		m_fPitch = XMConvertToRadians(-45.f);

	m_pBodyObject->Model_Upper_Rot(m_fPitch);

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

}
