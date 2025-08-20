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
    m_eAnimState = STATE_ANIM::START;
    if (pContainer->Get_WeaponType() == ENUM_CLASS(PLAYER_WEAPON::KNIFE))
    {
        m_bMotionSwap ? pContainer->Switch_Anim("Attack_1", false) : pContainer->Switch_Anim("Attack_2", false);
        m_bMotionSwap ? m_bMotionSwap = false : m_bMotionSwap = true;
    }
    else
    {
        pContainer->Switch_Anim("Aim_Shoot", false);
        m_pGameInstance->Publish(Hud_Weapon_Shoting{});
    }
}

void CAttack_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (pContainer->IsAnimFinsh() && m_eAnimState == STATE_ANIM::START)
    {
        if (pContainer->Get_AttackState().isAim)
            pContainer->Switch_State(TEXT("Aim"));
        else
            pContainer->Switch_State(TEXT("Idle"));
    }
}

void CAttack_Player::Exit(CPlayer* pContainer)
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
