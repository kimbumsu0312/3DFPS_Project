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

void CIdle_Player::Enter()
{
    m_eAnimState = STATE_ANIM::START;
    
    *m_pAnimTag = "Idle_Start";
    *m_pIsAnimLoop = false;
}

void CIdle_Player::Update(_float fDeltatime)
{
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::END)
    {
        switch (m_eInputKey)
        {
        case Client::CIdle_Player::INPUT_ENUM::MOVE_KEY:
            *m_pState = PLAYER_STATE::WALK;
            *m_pStateTag = TEXT("Walk");
            break;
        case Client::CIdle_Player::INPUT_ENUM::GUARD:
            *m_pState = PLAYER_STATE::GUARD;
            *m_pStateTag = TEXT("Guard");
            break;
        case Client::CIdle_Player::INPUT_ENUM::AIM:
            *m_pState = PLAYER_STATE::AIM;
            *m_pStateTag = TEXT("Aim");
            break;
        case Client::CIdle_Player::INPUT_ENUM::RELOAD:
            *m_pState = PLAYER_STATE::RELOAD;
            *m_pStateTag = TEXT("Reload");
            break;
        case Client::CIdle_Player::INPUT_ENUM::ATTACK:
            *m_pState = PLAYER_STATE::ATTACK;
            *m_pStateTag = TEXT("Attack");
            break;
        }
        return;
    }

    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::LOOP)
    {
        *m_pAnimTag = "Idle_Loop";
        *m_pIsAnimLoop = true;
    }
    KeyInput();
}

void CIdle_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

void CIdle_Player::KeyInput()
{
    if (m_eAnimState == STATE_ANIM::END || m_eAnimState == STATE_ANIM::START)
        return;

    if (m_pGameInstance->IsKeyHold(DIK_W) || m_pGameInstance->IsKeyHold(DIK_A) || m_pGameInstance->IsKeyHold(DIK_S) || m_pGameInstance->IsKeyHold(DIK_D))
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Idle_End";
        *m_pIsAnimLoop = false;

        m_eInputKey = INPUT_ENUM::MOVE_KEY;
    }
    else if(m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::RB) && *m_pWeaponState != PLAYER_WEAPON::KNIFE)
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Idle_End";
        *m_pIsAnimLoop = false;

        m_eInputKey = INPUT_ENUM::AIM;
    }
    else if (m_pGameInstance->IsKeyHold(DIK_E))
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Idle_End";
        *m_pIsAnimLoop = false;

        m_eInputKey = INPUT_ENUM::GUARD;
    }
    else if (m_pGameInstance->IsKeyHold(DIK_R) && *m_pWeaponState != PLAYER_WEAPON::KNIFE)
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Idle_End";
        *m_pIsAnimLoop = false;

        m_eInputKey = INPUT_ENUM::RELOAD;
    }
    else if (m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::LB))
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Idle_End";
        *m_pIsAnimLoop = false;

        m_eInputKey = INPUT_ENUM::ATTACK;
    }
    
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
