#include "pch.h"
#include "Reload_Player.h"
#include "Player.h"

CReload_Player::CReload_Player() : CStateObject()
{
}

HRESULT CReload_Player::Initalize(void* pArg)
{
    __super::Initalize(pArg);

    return S_OK;
}

void CReload_Player::Enter()
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

void CReload_Player::Update(_float fDeltatime)
{

    if (*m_pIsAnimFinsh && m_eReload_Type == RELOAD_TYPE::MAGAZINE)
    {
        *m_pState = PLAYER_STATE::IDLE;
        *m_pStateTag = TEXT("Idle");
        return;
    }
    
    if(m_eReload_Type == RELOAD_TYPE::BULLET)
    {
        if (m_eAnimState == STATE_ANIM::END && *m_pIsAnimFinsh)
        {
            *m_pState = PLAYER_STATE::IDLE;
            *m_pStateTag = TEXT("Idle");
            return;
        }

        if (*m_pIsAnimFinsh && m_eAnimState == STATE_ANIM::START)
            m_eAnimState = STATE_ANIM::LOOP;

        if(m_eAnimState == STATE_ANIM::LOOP)
            *m_pAnimTag = "Reload_Loop";

        KeyInput();
    }

}

void CReload_Player::Exit()
{
    m_eAnimState == STATE_ANIM::END;
}

void CReload_Player::KeyInput()
{
    if (m_eAnimState == STATE_ANIM::END || m_eAnimState == STATE_ANIM::START)
        return;

    if (m_pGameInstance->IsKeyHold(DIK_R))
    {
        m_eAnimState = STATE_ANIM::END;

        *m_pAnimTag = "Reload_End";
        *m_pIsAnimLoop = false;
    }


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
