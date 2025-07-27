#include "pch.h"
#include "Player.h"
#include "Body_Player.h"

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

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

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
	else
	{
		m_iState = ENUM_CLASS(PLAYER_STATE::IDLE);
	}
	m_PartObjects.at(TEXT("Part_Body"))->Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_PartObjects.at(TEXT("Part_Body"))->Late_Update(fTimeDelta);
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

	return S_OK;
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
}
