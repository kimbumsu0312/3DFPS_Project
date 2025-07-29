#include "pch.h"
#include "Attack_Player.h"
#include "Player.h"

CAttack_Player::CAttack_Player() : CStateObject()
{
}

HRESULT CAttack_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CAttack_Player::Enter()
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
    }
    *m_pIsAnimLoop = false;
}

void CAttack_Player::Update(_float fDeltatime)
{
    if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
    {
        *m_pState = PLAYER_STATE::IDLE;
        *m_pStateTag = TEXT("Idle");
        return;
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
