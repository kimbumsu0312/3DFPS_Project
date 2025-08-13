#include "pch.h"
#include "Idle_Player.h"
#include "Player.h"

CIdle_Player::CIdle_Player()
{
}

HRESULT CIdle_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CIdle_Player::Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    m_eAnimState = STATE_ANIM::LOOP;
    *m_pAnimTag = "Idle_Loop";
    *m_pIsAnimLoop = true;
}

void CIdle_Player::Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    //if(*m_pIsAnimFinsh)
    //    m_eAnimState = STATE_ANIM::LOOP;

    if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (pAttackState.isWeaponSwap)
            *m_pStateTag = TEXT("WeaponSwap");
        else if (pAttackState.isGuard)
            *m_pStateTag = TEXT("Guard");
        else if (pAttackState.isReload)
            *m_pStateTag = TEXT("Reload");
        else if (pAttackState.isAttack)
            *m_pStateTag = TEXT("Attack");
        else if (pAttackState.isAim)
            *m_pStateTag = TEXT("Aim");
        else
        {
            *m_pAnimTag = "Idle_Loop";
            *m_pIsAnimLoop = true;
        }
    }
}

void CIdle_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
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
