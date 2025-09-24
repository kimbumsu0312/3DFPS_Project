#include "pch.h"
#include "Attack_Player.h"
#include "Player.h"

CAttack_Player::CAttack_Player()
{
}

HRESULT CAttack_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack_Player::Enter(CPlayer* pContainer)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::KNIFE))
    {
        m_bMotionSwap ? pContainer->Switch_Anim("Attack_1", false) : pContainer->Switch_Anim("Attack_2", false);
        m_bMotionSwap ? m_bMotionSwap = false : m_bMotionSwap = true;
        m_pGameInstance->PlaySoundW(TEXT("Attack_Knife.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.7f);

    }
    else
    {
        switch (*pContainer->Get_BlackBoard()->Get_Data().iAnimState)
        {
        case ENUM_CLASS(PLAYER_ANIM::HANDGUN):
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("HandGun_Shot.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
            break;
        case ENUM_CLASS(PLAYER_ANIM::SHOTGUN):
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("Shotgun_Shot.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.3f);
            break;
        case ENUM_CLASS(PLAYER_ANIM::SNIPER):
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("Sniper_Shot.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
            break;
        }
        _int ItemIndex = CPlayer_Manager::GetInstance()->Get_SeleteItemIndex();
        _int iGunBullet = {};
        _int iInvenBullet = {};

        if (CInven_Manager::GetInstance()->Get_BulletCount(ItemIndex, iGunBullet, iInvenBullet))
            m_pGameInstance->Publish(Event_BulletCount_UI_OPEN{ ItemIndex, iGunBullet, iInvenBullet });

        pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
        pContainer->Switch_Anim("Aim_Shoot", false);
        m_pGameInstance->Publish(Hud_Weapon_Shoting{});
    }
}

void CAttack_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::KNIFE))
    {
        pContainer->AttackCollider();
    }
   if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
   {
       m_pGameInstance->Publish(Hud_Weapon_Shoting{});
       pContainer->Get_BlackBoard()->Set_Data().isBogan = true;

       pContainer->Get_BlackBoard()->Set_Data().isAttack = false;
       if (pContainer->Get_BlackBoard()->Get_Data().isAim == true)
       {
           pContainer->Get_BlackBoard()->Set_Data().isZoomOn = true;

       }
   }
}

void CAttack_Player::Exit(CPlayer* pContainer)
{
    CPlayer_Manager::GetInstance()->Set_KnifeAttack(false);
    m_eAnimState = STATE_ANIM::END;
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
