#include "pch.h"
#include "Idle_Player.h"
#include "Player.h"

CIdle_Player::CIdle_Player() : CStateObject()
{
}

HRESULT CIdle_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CIdle_Player::Enter(CContainerObject* pContainerObject)
{
}

void CIdle_Player::Update(CContainerObject* pContainerObject, _float fDeltatime)
{
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::JOG_F);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::JOG_L);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::JOG_R);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::WALK_B);
	}
	else if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::LB))
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::ATTACK);
	}
	else if (m_pGameInstance->IsKeyHold(DIK_E))
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::DIE);
	}
	else
	{
		*m_pState = ENUM_CLASS(CPlayer::PLAYER_STATE::IDLE);
	}
}

void CIdle_Player::Exit(CContainerObject* pContainerObject)
{
}

CIdle_Player* CIdle_Player::Create(void* pArg)
{
    CIdle_Player* pInstance = new CIdle_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_Player::Free()
{
	__super::Free();
}
