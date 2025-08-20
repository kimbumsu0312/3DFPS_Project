#include "pch.h"
#include "Attack_Daniela.h"
#include "Daniela.h"

CAttack_Daniela::CAttack_Daniela()
{
}

HRESULT CAttack_Daniela::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack_Daniela::Enter(CDaniela* pContainer)
{
    _int Rand = rand() % 3;
    m_eAnimState = STATE_ANIM::START;

    pContainer->Switch_AnimState(ENUM_CLASS(BOSS_SISTER::ATTACK));

    DIRECTION ePlayerDir = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));

    if (ePlayerDir == DIRECTION::F || ePlayerDir == DIRECTION::FR || ePlayerDir == DIRECTION::FL)
    {
        switch (Rand)
        {
        case 0:
            pContainer->Switch_Anim("Attack_1", false);
            break;
        case 1:
            pContainer->Switch_Anim("Attack_2", false);
            break;
        case 2:
            pContainer->Switch_Anim("Attack_3", false);
            break;
        }
    }
    else if(ePlayerDir == DIRECTION::L || ePlayerDir == DIRECTION::BL)
    {
        pContainer->Switch_Anim("Attack_Turn_L", false);
    }
    else if (ePlayerDir == DIRECTION::R || ePlayerDir == DIRECTION::BR || ePlayerDir == DIRECTION::B)
    {
        pContainer->Switch_Anim("Attack_Turn_R", false);
    }
}

void CAttack_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
    pContainer->Attack_Collision();
    if (pContainer->Get_State().isDamage)
    {
        pContainer->Switch_State(TEXT("Damage"));
        return;
    }

    if (pContainer->IsAnimFinsh())
    {
        pContainer->Switch_State(TEXT("Chase"));
    }
}

void CAttack_Daniela::Exit(CDaniela* pContainer)
{
}

CAttack_Daniela* CAttack_Daniela::Create(void* pArg)
{
    CAttack_Daniela* pInstance = new CAttack_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_Daniela::Free()
{
    __super::Free();
}
