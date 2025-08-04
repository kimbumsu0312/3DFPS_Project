#include "pch.h"
#include "Reload_Player.h"
#include "Player.h"

CReload_Player::CReload_Player()
{
}

HRESULT CReload_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CReload_Player::Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    if (*m_pWeaponState == PLAYER_WEAPON::SHOTGUN)
    {
        m_eReload_Type = RELOAD_TYPE::BULLET;
        *m_pAnimTag = "Reload_Start";
    }
    else
    {
        m_eReload_Type = RELOAD_TYPE::MAGAZINE;
        *m_pAnimTag = "Reload";
    }
    m_eAnimState = STATE_ANIM::START;

    *m_pIsAnimLoop = false;
}

void CReload_Player::Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    if ( m_eReload_Type == RELOAD_TYPE::MAGAZINE)
    {
        if (*m_pIsAnimFinsh)
        {
            if (pAttackState.isAim)
                *m_pStateTag = TEXT("Aim");
            else
                *m_pStateTag = TEXT("Idle");
        }
        return;
    }
    else if (m_eReload_Type == RELOAD_TYPE::BULLET)
    {
        if (m_eAnimState == STATE_ANIM::START && *m_pIsAnimFinsh)
        {
            m_eAnimState = STATE_ANIM::LOOP;
            *m_pAnimTag = "Reload_Loop";
        }
        else if (m_eAnimState == STATE_ANIM::LOOP && *m_pIsAnimFinsh)
        {
            m_eAnimState = STATE_ANIM::END;
            *m_pAnimTag = "Reload_End";
        }
        else if (m_eAnimState == STATE_ANIM::END && *m_pIsAnimFinsh)
        {
            if (pAttackState.isAim)
                *m_pStateTag = TEXT("Aim");
            else
                *m_pStateTag = TEXT("Idle");
        }
    }
}

void CReload_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

CReload_Player* CReload_Player::Create(void* pArg)
{
    CReload_Player* pInstance = new CReload_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CReload_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CReload_Player::Free()
{
    __super::Free();
}
