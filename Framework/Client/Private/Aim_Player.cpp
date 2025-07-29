#include "pch.h"
#include "Aim_Player.h"
#include "Player.h"

CAim_Player::CAim_Player() : CStateObject()
{
}

HRESULT CAim_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CAim_Player::Enter()
{
    m_eAnimState = STATE_ANIM::START;

    *m_pAnimTag = "Aim_Start";
    *m_pIsAnimLoop = false;
}

void CAim_Player::Update(_float fDeltatime)
{
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::END)
    {
        *m_pState = PLAYER_STATE::IDLE;
        *m_pStateTag = TEXT("Idle");
        return;
    }

    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::LOOP)
    {
        *m_pAnimTag = "Aim_Loop";
        *m_pIsAnimLoop = true;
    }
    KeyInput();
}

void CAim_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

void CAim_Player::KeyInput()
{
    if (m_eAnimState == STATE_ANIM::END || m_eAnimState == STATE_ANIM::START)
        return;

    if (m_pGameInstance->IsMouseUp(MOUSEKEYSTATE::RB))
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Aim_End";
        *m_pIsAnimLoop = false;
    }
}

CAim_Player* CAim_Player::Create(void* pArg)
{
    CAim_Player* pInstance = new CAim_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAim_Player::Free()
{
    __super::Free();
}
