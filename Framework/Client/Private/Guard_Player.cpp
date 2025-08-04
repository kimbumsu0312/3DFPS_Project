#include "pch.h"
#include "Guard_Player.h"
#include "Player.h"

CGuard_Player::CGuard_Player()
{
}

HRESULT CGuard_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CGuard_Player::Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    m_eAnimState = STATE_ANIM::LOOP;

    *m_pAnimTag = "Guard_Loop";
    *m_pIsAnimLoop = true;
}

void CGuard_Player::Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    if (!pAttackState.isGuard && pAttackState.isAim)
        *m_pStateTag = TEXT("Aim");
    else if(!pAttackState.isGuard)
        *m_pStateTag = TEXT("Idle");
    else
    {
        if(pMoveState.isMoveB || pMoveState.isMoveF || pMoveState.isMoveL || pMoveState.isMoveR)
            *m_pAnimTag = "Guard_Walk";
        else
            *m_pAnimTag = "Guard_Loop";
    }
}

void CGuard_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
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
