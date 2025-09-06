#include "pch.h"
#include "Attack_Bela.h"
#include "Bela.h"

CAttack_Bela::CAttack_Bela()
{
}

HRESULT CAttack_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack_Bela::Enter(CBela* pContainer)
{
    _int Rand = rand() % 3;
    m_eAnimState = STATE_ANIM::START;

    //pContainer->Switch_AnimState(ENUM_CLASS(CBela::BOSS_SISTER::ATTACK));

    //DIRECTION ePlayerDir = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));

    //if (ePlayerDir == DIRECTION::F || ePlayerDir == DIRECTION::FR || ePlayerDir == DIRECTION::FL)
    //{
    //    switch (Rand)
    //    {
    //    case 0:
    //        pContainer->Switch_Anim("Freeszes_Swing", false);
    //        break;
    //    case 1:
    //        pContainer->Switch_Anim("Freeszes_Attack_CriticalHit", false);
    //        break;
    //    case 2:
    //        pContainer->Switch_Anim("Freeszes_MadHold_CriticalHit", false);
    //        break;
    //    }
    //}
    //else if (ePlayerDir == DIRECTION::L || ePlayerDir == DIRECTION::BL)
    //{
    //    pContainer->Switch_Anim("Freeszes_Attack_Turn_L", false);
    //}
    //else if (ePlayerDir == DIRECTION::R || ePlayerDir == DIRECTION::BR || ePlayerDir == DIRECTION::B)
    //{
    //    pContainer->Switch_Anim("Freeszes_Attack_Turn_R", false);
    //}
}

void CAttack_Bela::Update(CBela* pContainer, _float fDeltatime)
{

}

void CAttack_Bela::Exit(CBela* pContainer)
{
}

CAttack_Bela* CAttack_Bela::Create(void* pArg)
{
    CAttack_Bela* pInstance = new CAttack_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_Bela::Free()
{
    __super::Free();
}
