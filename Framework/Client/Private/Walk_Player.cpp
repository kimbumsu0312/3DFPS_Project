#include "pch.h"
#include "Walk_Player.h"
#include "Player.h"

CWalk_Player::CWalk_Player() : CStateObject()
{
}

HRESULT CWalk_Player::Initalize(void* pArg)
{
	__super::Initalize(pArg);

	return S_OK;
}

void CWalk_Player::Enter()
{
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_eMoveKey = MOVE_SATATE::SF;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_eMoveKey = MOVE_SATATE::SL;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_eMoveKey = MOVE_SATATE::SR;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_eMoveKey = MOVE_SATATE::SB;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_eMoveKey = MOVE_SATATE::F;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_eMoveKey = MOVE_SATATE::L;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_eMoveKey = MOVE_SATATE::R;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_eMoveKey = MOVE_SATATE::B;
	}

	AnimSet();
}

void CWalk_Player::Update(_float fDeltatime)
{
	KeyInput(fDeltatime);
	AnimSet();
}

void CWalk_Player::Exit()
{
	*m_pAnimTag = "Walk_Loop";
}

void CWalk_Player::AnimSet()
{
	if (m_eMoveKey == MOVE_SATATE::SF)
	{
		*m_pAnimTag = "Jog_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::SR)
	{
		*m_pAnimTag = "Jog_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::SL)
	{
		*m_pAnimTag = "Jog_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::SB)
	{
		*m_pAnimTag = "Jog_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::F)
	{
		*m_pAnimTag = "Walk_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::R)
	{
		*m_pAnimTag = "Walk_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::L)
	{
		*m_pAnimTag = "Walk_Loop";
		*m_pIsAnimLoop = true;
	}
	else if (m_eMoveKey == MOVE_SATATE::B)
	{
		*m_pAnimTag = "Walk_Loop";
		*m_pIsAnimLoop = true;
	}
	
}

void CWalk_Player::KeyInput(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyHold(DIK_W))
	{
		m_eMoveKey = MOVE_SATATE::F;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_A))
	{
		m_eMoveKey = MOVE_SATATE::L;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_D))
	{
		m_eMoveKey = MOVE_SATATE::R;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_S))
	{
		m_eMoveKey = MOVE_SATATE::B;
	}
	else 
	{

		*m_pState = PLAYER_STATE::IDLE;
		*m_pStateTag = TEXT("Idle");
		m_eMoveKey = MOVE_SATATE::END;
	}
}

CWalk_Player* CWalk_Player::Create(void* pArg)
{
	CWalk_Player* pInstance = new CWalk_Player();

	if (FAILED(pInstance->Initalize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CWalk_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWalk_Player::Free()
{
	__super::Free();
}
