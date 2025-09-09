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


    if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::HANDGUN))
    {
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(PLAYER_ANIM::WEAPONCHANGE);
        pContainer->Switch_Anim("Weapon_Change_HandGun", false);
    }
    else if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SHOTGUN))
    {
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(PLAYER_ANIM::WEAPONCHANGE);
        pContainer->Switch_Anim("Weapon_Change_ShotGun", false);
    }
    else if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SNIPER))
    {
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(PLAYER_ANIM::WEAPONCHANGE);
        pContainer->Switch_Anim("Weapon_Change_Sniper", false);
    }
    else if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::KNIFE))
    {
        *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(PLAYER_ANIM::NONE);
        pContainer->Switch_Anim("Walk_Loop", false);
    }
    else
    {
        pContainer->WeaponSwap();
    }
}

void CWeaponSwap_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
   if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true )
   {
        pContainer->WeaponSwap();
   }
}

void CWeaponSwap_Player::Exit(CPlayer* pContainer)
{

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
