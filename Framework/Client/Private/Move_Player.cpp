#include "pch.h"
#include "Move_Player.h"
#include "Player.h"

CMove_Player::CMove_Player()
{
}

HRESULT CMove_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CMove_Player::Enter(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    if (pContainer->Get_WeaponType() == ENUM_CLASS(PLAYER_WEAPON::SNIPER))
    {
        pContainer->Switch_Anim("Idle_Loop", true);
    }
    else
    {
        pContainer->Switch_Anim("Walk_Loop", true);
    }
}

void CMove_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (pContainer->Get_AttackState().isWeaponSwap)
            pContainer->Switch_State(TEXT("WeaponSwap"));
        else if (pContainer->Get_AttackState().isGuard)
            pContainer->Switch_State(TEXT("Guard"));
        else if (pContainer->Get_AttackState().isReload)
            pContainer->Switch_State(TEXT("Reload"));
        else if (pContainer->Get_AttackState().isAttack)
            pContainer->Switch_State(TEXT("Attack"));
        else if (pContainer->Get_AttackState().isAim)
            pContainer->Switch_State(TEXT("Aim"));
        else if (!pContainer->Get_AttackState().isMove)
            pContainer->Switch_State(TEXT("Idle"));
    }
}

void CMove_Player::Exit(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}


CMove_Player* CMove_Player::Create(void* pArg)
{
    CMove_Player* pInstance = new CMove_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CMove_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMove_Player::Free()
{
    __super::Free();
}
