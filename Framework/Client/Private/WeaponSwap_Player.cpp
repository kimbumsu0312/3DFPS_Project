#include "pch.h"
#include "WeaponSwap_Player.h"
#include "Player.h"

CWeaponSwap_Player::CWeaponSwap_Player()
{
}

HRESULT CWeaponSwap_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CWeaponSwap_Player::Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    m_eAnimState = STATE_ANIM::START;
    if(*m_pWeaponState == ENUM_CLASS(PLAYER_WEAPON::HANDGUN) || *m_pWeaponState == ENUM_CLASS(PLAYER_WEAPON::KNIFE))
        *m_pAnimTag = "Weapon_Change_HandGun";
    else if (*m_pWeaponState == ENUM_CLASS(PLAYER_WEAPON::SHOTGUN))
        *m_pAnimTag = "Weapon_Change_ShotGun";
    else if (*m_pWeaponState == ENUM_CLASS(PLAYER_WEAPON::SNIPER))
        *m_pAnimTag = "Weapon_Change_Sniper";
    *m_pWeaponState = PLAYER_WEAPON::WEAPONCHANGE;

    *m_pIsAnimLoop = false;
}

void CWeaponSwap_Player::Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState)
{
    if (*m_pIsAnimFinsh)
    {
        *m_pWeaponState = *m_pNextWeaponState;
        *m_pStateTag = TEXT("Idle");
    }
}

void CWeaponSwap_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

CWeaponSwap_Player* CWeaponSwap_Player::Create(void* pArg)
{
    CWeaponSwap_Player* pInstance = new CWeaponSwap_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CWeaponSwap_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeaponSwap_Player::Free()
{
    __super::Free();
}
