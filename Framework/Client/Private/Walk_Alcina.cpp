#include "pch.h"
#include "Walk_Alcina.h"
#include "Player_Manager.h"

CWalk_Alcina::CWalk_Alcina()
{
}

HRESULT CWalk_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CWalk_Alcina::Enter(CAlcina* pContainer)
{
    if(!m_bIsLight)
       m_pGameInstance->OnOff_Light(TEXT("Light_Alchina"), true);

    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::NORMAL);
    DIRECTION eDir = PlayerDIR(pContainer->Get_TransForm()->Get_State(STATE::POSITION), pContainer->Get_TransForm()->Get_State(STATE::LOOK));
    if(eDir == DIRECTION::F || eDir == DIRECTION::FL || eDir == DIRECTION::FR)
        pContainer->Switch_Anim("Walk_F", false);
    else if (eDir == DIRECTION::B || eDir == DIRECTION::BL || eDir == DIRECTION::BR)
        pContainer->Switch_Anim("Walk_F", false);
    else if (eDir == DIRECTION::L)
        pContainer->Switch_Anim("Walk_L", false);
    else if (eDir == DIRECTION::R)
        pContainer->Switch_Anim("Walk_R", false);

}

void CWalk_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        _bool AnimFinsh = *pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh;
        if (AnimFinsh)
        {
            m_eAnimState = STATE_ANIM::LOOP;
            *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::NORMAL);
            pContainer->Switch_Anim("Walk_Loop", true);
        }
        pContainer->Target_LookTurn(fDeltatime);
    }

    if (m_eAnimState == STATE_ANIM::LOOP)
    {
        _int iTargetCellIndex = CPlayer_Manager::GetInstance()->Get_CellIndex();

        _float3 vPlayerPos{};
        XMStoreFloat3(&vPlayerPos, CPlayer_Manager::GetInstance()->Get_PlayerPos());
        if (m_iPreTargetIndex != iTargetCellIndex)
        {
            m_iPreTargetIndex = iTargetCellIndex;
 
        }
     
    }
}

void CWalk_Alcina::Exit(CAlcina* pContainer)
{
}

CWalk_Alcina* CWalk_Alcina::Create(void* pArg)
{
    CWalk_Alcina* pInstance = new CWalk_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CWalk_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWalk_Alcina::Free()
{
    __super::Free();
}
