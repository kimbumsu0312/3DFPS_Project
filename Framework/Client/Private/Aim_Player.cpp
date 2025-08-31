#include "pch.h"
#include "Aim_Player.h"
#include "Player.h"

CAim_Player::CAim_Player()
{
}

HRESULT CAim_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CAim_Player::Enter(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_Anim("Aim_Loop", true);
}

void CAim_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (pContainer->Get_AttackState().isGuard)
        pContainer->Switch_State(TEXT("Guard"));
    else if (pContainer->Get_AttackState().isReload)
        pContainer->Switch_State(TEXT("Reload"));
    else if (pContainer->Get_AttackState().isAttack)
        pContainer->Switch_State(TEXT("Attack"));
    else if (!pContainer->Get_AttackState().isAim)
        pContainer->Switch_State(TEXT("Idle"));
}

void CAim_Player::Exit(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}

CAim_Player* CAim_Player::Create(void* pArg)
{
    CAim_Player* pInstance = new CAim_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAim_Player::Free()
{
    __super::Free();
}
