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
        if (pContainer->IsAnimFinsh())
        {
            pContainer->Get_BlackBoard()->Set_Data().isReload = false;
        }
        return;
    }
    else if (m_eReload_Type == RELOAD_TYPE::BULLET)
    {
        if (m_eAnimState == STATE_ANIM::START && pContainer->IsAnimFinsh())
        {
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
            pContainer->Get_BlackBoard()->Set_Data().isReload = false;
        }
    }
}

void CReload_Player::Exit(CPlayer* pContainer)
{
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
