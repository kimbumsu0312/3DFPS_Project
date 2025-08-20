#include "pch.h"
#include "WeaponSwap_Player.h"
#include "Player.h"

CWeaponSwap_Player::CWeaponSwap_Player()
{
}

HRESULT CWeaponSwap_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CWeaponSwap_Player::Enter(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    if (pContainer->Get_WeaponType() == ENUM_CLASS(PLAYER_WEAPON::HANDGUN) || pContainer->Get_WeaponType() == ENUM_CLASS(PLAYER_WEAPON::KNIFE))
        pContainer->Switch_Anim("Weapon_Change_HandGun", false);
    else if (pContainer->Get_WeaponType() == ENUM_CLASS(PLAYER_WEAPON::SHOTGUN))
        pContainer->Switch_Anim("Weapon_Change_ShotGun", false);
    else if (pContainer->Get_WeaponType() == ENUM_CLASS(PLAYER_WEAPON::SNIPER))
        pContainer->Switch_Anim("Weapon_Change_Sniper", false);

    pContainer->Switch_WeaponState(ENUM_CLASS(PLAYER_WEAPON::WEAPONCHANGE));
}

void CWeaponSwap_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (pContainer->IsAnimFinsh())
    {
        pContainer->WeaponSwap();
        pContainer->Switch_State(TEXT("Idle"));
    }
}

void CWeaponSwap_Player::Exit(CPlayer* pContainer)
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
