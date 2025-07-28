#include "pch.h"
#include "Move_Player.h"
#include "Player.h"

CMove_Player::CMove_Player() : CStateObject()
{
}

HRESULT CMove_Player::Initalize(void* pArg)
{
	MOVE_PLAYER_DESC* pDesc = static_cast<MOVE_PLAYER_DESC*>(pArg);

	m_pTransForm = pDesc->pTransForm;

	Safe_AddRef(m_pTransForm);
	__super::Initalize(pArg);

	return S_OK;
}

void CMove_Player::Enter(CContainerObject* pContainerObject)
{

}

void CMove_Player::Update(CContainerObject* pContainerObject, _float fDeltatime)
{
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_pTransForm->Go_Straight(fDeltatime);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_pTransForm->Go_Left(fDeltatime);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_pTransForm->Go_Right(fDeltatime);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_pTransForm->Go_Backward(fDeltatime);
	}
	else
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::IDLE);
	}

}

void CMove_Player::Exit(CContainerObject* pContainerObject)
{
}

CMove_Player* CMove_Player::Create(void* pArg)
{
	CMove_Player* pInstance = new CMove_Player();

	if (FAILED(pInstance->Initalize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CMove_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMove_Player::Free()
{
	__super::Free();
}
