#include "pch.h"
#include "Guard_Player.h"
#include "Player.h"

CGuard_Player::CGuard_Player() : CStateObject()
{
}

HRESULT CGuard_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CGuard_Player::Enter()
{
    m_eAnimState = STATE_ANIM::START;

    *m_pAnimTag = "Guard_Start";
    *m_pIsAnimLoop = false;
}

void CGuard_Player::Update(_float fDeltatime)
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
        if (m_pGameInstance->IsKeyHold(DIK_W) || m_pGameInstance->IsKeyHold(DIK_A) || m_pGameInstance->IsKeyHold(DIK_S) || m_pGameInstance->IsKeyHold(DIK_D))
        {
            *m_pAnimTag = "Guard_Walk";
            *m_pIsAnimLoop = true;
        }
        else
        {
            *m_pAnimTag = "Guard_Loop";
            *m_pIsAnimLoop = true;
        }
    }
    KeyInput();
}

void CGuard_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

void CGuard_Player::KeyInput()
{
    if (m_eAnimState == STATE_ANIM::END || m_eAnimState == STATE_ANIM::START)
        return;

    if (m_pGameInstance->IsKeyUp(DIK_E))
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Guard_End";
        *m_pIsAnimLoop = false;
    }


}

CGuard_Player* CGuard_Player::Create(void* pArg)
{
    CGuard_Player* pInstance = new CGuard_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CGuard_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGuard_Player::Free()
{
    __super::Free();
}
