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
    if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::NONE))
        pContainer->Switch_Anim("Walk_Loop", true);
    else
        pContainer->Switch_Anim("Idle_Loop", true);
}

void CIdle_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{

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
