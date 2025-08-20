#include "pch.h"
#include "Guard_Player.h"
#include "Player.h"

CGuard_Player::CGuard_Player()
{
}

HRESULT CGuard_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CGuard_Player::Enter(CPlayer* pContainer)
{
    m_eAnimState = STATE_ANIM::LOOP;
    pContainer->Switch_Anim("Guard_Loop", true);
}

void CGuard_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (pContainer->Get_AttackState().isGuard && pContainer->Get_AttackState().isAim)
        pContainer->Switch_State(TEXT("Aim"));
    else if (!pContainer->Get_AttackState().isGuard)
        pContainer->Switch_State(TEXT("Idle"));
    else
    {
        if (pContainer->Get_MoveState().isMoveB || pContainer->Get_MoveState().isMoveF || 
            pContainer->Get_MoveState().isMoveL || pContainer->Get_MoveState().isMoveR)
            pContainer->Switch_Anim("Guard_Walk", true);
        else
            pContainer->Switch_Anim("Guard_Loop", true);
    }
}

void CGuard_Player::Exit(CPlayer* pContainer)
{
    m_eAnimState == STATE_ANIM::END;
}

CGuard_Player* CGuard_Player::Create(void* pArg)
{
    CGuard_Player* pInstance = new CGuard_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CGuard_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGuard_Player::Free()
{
    __super::Free();
}
