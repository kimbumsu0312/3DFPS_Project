#include "pch.h"
#include "Reload_Player.h"
#include "Player.h"

CReload_Player::CReload_Player()
{
}

HRESULT CReload_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CReload_Player::Enter(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SHOTGUN))
    {
        m_eReload_Type = RELOAD_TYPE::BULLET;
        pContainer->Switch_Anim("Reload_Start", false);
    }
    else
    {
        m_eReload_Type = RELOAD_TYPE::MAGAZINE;
        pContainer->Switch_Anim("Reload", false);
    }
}

void CReload_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (m_eReload_Type == RELOAD_TYPE::MAGAZINE)
    {
        m_fAccTime += fTimeDelta;
        if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SNIPER))
        {
            if (m_fAccTime > 1.1f && m_iCount == 0)
            {
                ++m_iCount;
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
                m_pGameInstance->PlaySoundW(TEXT("Sniper_Reload_1.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
            }
            else if (m_fAccTime > 3.5f && m_iCount == 1)
            {
                ++m_iCount;
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
                m_pGameInstance->PlaySoundW(TEXT("Sniper_Reload_2.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
            }
        }
        else if(*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::HANDGUN))
        {
            if (m_fAccTime > 1.8f && m_iCount == 0)
            {
                ++m_iCount;
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
                m_pGameInstance->PlaySoundW(TEXT("HandGun_Reload_1.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
            }
            else if (m_fAccTime > 3.55f && m_iCount == 1)
            {
                ++m_iCount;
                m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
                m_pGameInstance->PlaySoundW(TEXT("HandGun_Reload_2.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
            }
        }
        if (pContainer->IsAnimFinsh())
        {
            pContainer->Get_BlackBoard()->Set_Data().isReload = false;
        }
        return;
    }
    else if (m_eReload_Type == RELOAD_TYPE::BULLET)
    {
        m_fAccTime += fTimeDelta;
        if (m_fAccTime > 1.45f && m_iCount == 0)
        {
            ++m_iCount;
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("ShotGun_Reload_1.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
        }
        else if (m_fAccTime > 2.9f && m_iCount == 1)
        {
            ++m_iCount;
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("ShotGun_Reload_1.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
        }
        else if (m_fAccTime > 4.2f && m_iCount == 2)
        {
            ++m_iCount;
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("ShotGun_Reload_2.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
        }

        if (m_eAnimState == STATE_ANIM::START && pContainer->IsAnimFinsh())
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
            m_eAnimState = STATE_ANIM::LOOP;
            pContainer->Switch_Anim("Reload_Loop", false);
        }
        else if (m_eAnimState == STATE_ANIM::LOOP && pContainer->IsAnimFinsh())
        {
            m_eAnimState = STATE_ANIM::END;
            pContainer->Switch_Anim("Reload_End", false);
        }
        else if (m_eAnimState == STATE_ANIM::END && pContainer->IsAnimFinsh())
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = true;
            pContainer->Get_BlackBoard()->Set_Data().isReload = false;
        }
    }
}

void CReload_Player::Exit(CPlayer* pContainer)
{
    m_fAccTime = 0.f;
    m_iCount = 0;
    _int ItemIndex = CPlayer_Manager::GetInstance()->Get_SeleteItemIndex();
    _int iGunBullet = {};
    _int iInvenBullet = {};

    if (CInven_Manager::GetInstance()->Get_BulletCount(ItemIndex, iGunBullet, iInvenBullet))
        m_pGameInstance->Publish(Event_BulletCount_UI_OPEN{ ItemIndex, iGunBullet, iInvenBullet });

    m_eAnimState = STATE_ANIM::END;
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
