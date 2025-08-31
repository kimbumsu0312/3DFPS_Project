#include "pch.h"
#include "Idle_Player.h"
#include "Player.h"

CIdle_Player::CIdle_Player()
{
}

HRESULT CIdle_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CIdle_Player::Enter(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_Anim("Idle_Loop", true);
}

void CIdle_Player::Update(CPlayer* pContainer, _float fTimeDelta)
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
        else if (pContainer->Get_AttackState().isMove)
            pContainer->Switch_State(TEXT("Move"));
    }
}

void CIdle_Player::Exit(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}


CIdle_Player* CIdle_Player::Create(void* pArg)
{
    CIdle_Player* pInstance = new CIdle_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CIdle_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIdle_Player::Free()
{
	__super::Free();
}
