#include "pch.h"
#include "Attack_Player.h"
#include "Player.h"

CAttack_Player::CAttack_Player()
{
}

HRESULT CAttack_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CAttack_Player::Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    m_eAnimState = STATE_ANIM::START;
    if (*m_pWeaponState == PLAYER_WEAPON::KNIFE)
    {
        m_bAttackPatten ? *m_pAnimTag = "Attack_1" : *m_pAnimTag = "Attack_2";
        m_bAttackPatten ? m_bAttackPatten = false : m_bAttackPatten = true;
    }
    else
    {
        *m_pAnimTag = "Aim_Shoot";
        m_pGameInstance->Publish(Hud_Weapon_Shoting{});
    }
    *m_pIsAnimLoop = false;
}

void CAttack_Player::Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
    {
        if(pAttackState.isAim)
            *m_pStateTag = TEXT("Aim");
        else
            *m_pStateTag = TEXT("Idle");
    }
}

void CAttack_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

CAttack_Player* CAttack_Player::Create(void* pArg)
{
    CAttack_Player* pInstance = new CAttack_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_Player::Free()
{
    __super::Free();
}
