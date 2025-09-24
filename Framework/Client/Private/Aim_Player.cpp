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
    m_eAnimState = STATE_ANIM::START;
    if (pContainer->Get_BlackBoard()->Get_Data().isZoomOn == true)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SHOTGUN))
            pContainer->Switch_Anim("Aim_Start", true);
        else
         pContainer->Switch_Anim("Aim_Loop", true);
        m_eAnimState = STATE_ANIM::LOOP;
        pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
        pContainer->Get_BlackBoard()->Set_Data().isZoomOn = false;
    }
    else
    {
        m_eAnimState = STATE_ANIM::START;
        pContainer->Switch_Anim("Aim_Start", false);
    }
}

void CAim_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
            m_eAnimState = STATE_ANIM::LOOP;
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if(*pContainer->Get_BlackBoard()->Get_Data().iAnimState == ENUM_CLASS(PLAYER_ANIM::SHOTGUN))
            pContainer->Switch_Anim("Aim_Start", true);
        else
            pContainer->Switch_Anim("Aim_Loop", true);
    }

}

void CAim_Player::Exit(CPlayer* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;
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
