#include "pch.h"
#include "Player.h"
#include "Client_Enum.h"
#include "Body_Player.h"
#include "Camera_Player.h"
#include "Model.h"
#include "Knife.h"

#include "Idle_Player.h"
#include "Walk_Player.h"
#include "Guard_Player.h"
#include "Aim_Player.h"
#include "Reload_Player.h"
#include "Attack_Player.h"

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
	m_iWeponState = PLAYER_WEAPON::SNIPER;
	m_szAnimTag = "Idle_Loop";
	m_szStateTag = TEXT("Idle");
	m_iCulState = PLAYER_STATE::IDLE;
	m_iPreState = m_iCulState;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;
	m_pMovePos = static_cast<CBody_Player*>(m_PartObjects.at(TEXT("Part_Body")))->Get_MovePos();

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 3.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_pCamera->Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
 {
	if (m_pGameInstance->IsKeyDown(DIK_1))
		m_iWeponState = PLAYER_WEAPON::KNIFE;
	if (m_pGameInstance->IsKeyDown(DIK_2))
		m_iWeponState = PLAYER_WEAPON::SHOTGUN;
	if (m_pGameInstance->IsKeyDown(DIK_3))
		m_iWeponState = PLAYER_WEAPON::SNIPER;
	if (m_pGameInstance->IsKeyDown(DIK_4))
		m_iWeponState = PLAYER_WEAPON::HANDGUN;


 	m_CulStateObject->Update( fTimeDelta);
	if (m_iPreState != m_iCulState)
	{
		m_CulStateObject->Exit();
		Safe_Release(m_CulStateObject);

		m_CulStateObject = Find_StateObject(m_szStateTag);
		Safe_AddRef(m_CulStateObject);

		m_CulStateObject->Enter();
		m_iPreState = m_iCulState;
	}

	m_bIsAnimFinsh = false;
	Move(fTimeDelta);
	__super::Update(fTimeDelta);
	m_pCamera->Update(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pCamera->Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return S_OK;
}

void CPlayer::Move(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

}

HRESULT CPlayer::Ready_Components()
{

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_DESC BodyDesc{};
	BodyDesc.pState = &m_iCulState;
	BodyDesc.pWeaponState = &m_iWeponState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pAnimTag = &m_szAnimTag;
	BodyDesc.pIsAnimLoop = &m_bIsAnimLoop;
	BodyDesc.pIsAnimFinsh = &m_bIsAnimFinsh;

	if(FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;
	
	CBody_Player* pBody = static_cast<CBody_Player*>(Find_PartObject(TEXT("Part_Body")));
	
	if (pBody == nullptr)
		return E_FAIL;
	CKnife::KNIFE_DESC	KnifeDesc{};
	
	KnifeDesc.pState = &m_iCulState;
	KnifeDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	KnifeDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_MiddleF3"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_Knife"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Knife_Player"), &KnifeDesc)))
		return E_FAIL;

	CCamera_Player::CAMERA_PLAYER_DESC CameraDesc{};
	CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, -1.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fMouseSensor = 0.2f;
	CameraDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("Cam"));
	CameraDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	m_pCamera = dynamic_cast<CCamera_Player*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc));

	return S_OK;
}

HRESULT CPlayer::Ready_StateObjects()
{
	CStateObject::STATE_DESC Desc{};
	Desc.pState = &m_iCulState;
	Desc.pWeaponState = &m_iWeponState;
	Desc.pAnimTag = &m_szAnimTag;
	Desc.pStateTag = &m_szStateTag;
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
	Add_StateObject(TEXT("Walk"), CWalk_Player::Create(&Desc));
	Add_StateObject(TEXT("Aim"), CAim_Player::Create(&Desc));
	Add_StateObject(TEXT("Reload"), CReload_Player::Create(&Desc));
	Add_StateObject(TEXT("Attack"), CAttack_Player::Create(&Desc));
	return S_OK;
}

void CPlayer::Update_RootMove()
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(m_pTransformCom->Get_State(STATE::POSITION) + XMLoadFloat3(m_pMovePos), 1.f));
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

	for (auto& Pair : m_StateObjects)
		Safe_Release(Pair.second);

	m_StateObjects.clear();
	Safe_Release(m_CulStateObject);
}
