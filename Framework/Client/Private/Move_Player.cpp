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
    if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::NONE))
        pContainer->Switch_Anim("Walk_Loop", true);
    else  if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SNIPER))
        pContainer->Switch_Anim("Idle_Loop", true);
    else
        pContainer->Switch_Anim("Walk_Loop", true);
}

void CMove_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
}

void CMove_Player::Exit(CPlayer* pContainer)
{
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
