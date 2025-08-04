#include "pch.h"
#include "Aim_Player.h"
#include "Player.h"

CAim_Player::CAim_Player()
{
}

HRESULT CAim_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CAim_Player::Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    m_eAnimState = STATE_ANIM::LOOP;

    *m_pAnimTag = "Aim_Loop";
    *m_pIsAnimLoop = true;
}

void CAim_Player::Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    if(pAttackState.isGuard)
        *m_pStateTag = TEXT("Guard");
    else if (pAttackState.isReload)
        *m_pStateTag = TEXT("Reload");
    else if (pAttackState.isAttack)
        *m_pStateTag = TEXT("Attack");
    else if (!pAttackState.isAim)
        *m_pStateTag = TEXT("Idle");
    else
    {
        *m_pAnimTag = "Aim_Loop";
        *m_pIsAnimLoop = true;
    }

}

void CAim_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
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
