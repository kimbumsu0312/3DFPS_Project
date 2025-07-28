#include "pch.h"
#include "Player.h"
#include "Body_Player.h"
#include "Camera_Player.h"
#include "Model.h"
#include "Knife.h"
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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
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
 	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::JOG_F);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::JOG_L);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::JOG_R);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::WALK_B);
	}
	else if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::LB))
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::ATTACK);
	}
	else
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::IDLE);
	}
	__super::Update(fTimeDelta);
	m_pCamera->Update(fTimeDelta);
	//Update_RootMove();
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

HRESULT CPlayer::Ready_Components()
{

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_DESC BodyDesc{};
	BodyDesc.pState = &m_iState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if(FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;
	
	CBody_Player* pBody = static_cast<CBody_Player*>(Find_PartObject(TEXT("Part_Body")));
	
	if (pBody == nullptr)
		return E_FAIL;
	CKnife::KNIFE_DESC	KnifeDesc{};
	
	KnifeDesc.pState = &m_iState;
	KnifeDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	KnifeDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_MiddleF1"));
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

void CPlayer::Update_RootMove()
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(m_pTransformCom->Get_State(STATE::POSITION) + XMLoadFloat3(m_pMovePos), 1.f));
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
}
