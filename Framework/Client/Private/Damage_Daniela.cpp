#include "pch.h"
#include "Damage_Daniela.h"
#include "Daniela.h"

CDamage_Daniela::CDamage_Daniela()
{
}

HRESULT CDamage_Daniela::Initalize(void* pArg)
{
    return S_OK;
}

void CDamage_Daniela::Enter(CDaniela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    pContainer->Switch_AnimState(ENUM_CLASS(BOSS_SISTER::DAMAGE));
    pContainer->IsDamage();

    if(CPlayer_Manager::GetInstance()->Get_Damage() >= 30)
        Big_Damage(pContainer);
    else
        Normal_Damage(pContainer);

}

void CDamage_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
    if (pContainer->IsAnimFinsh())
    {
       if (pContainer->Get_State().isChase)
       {
           pContainer->Switch_State(TEXT("Chase"));
       }
       else
       {
           pContainer->Switch_State(TEXT("Idle"));
       }
        m_eAnimState = STATE_ANIM::LOOP;
    }
}

void CDamage_Daniela::Exit(CDaniela* pContainer)
{
    DIRECTION eHitPoint = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));
    if (eHitPoint == DIRECTION::B)
        pContainer->Target_LookAt();

    m_eAnimState = STATE_ANIM::END;
    pContainer->Reset_DamageCheck();
}

void CDamage_Daniela::Normal_Damage(CDaniela* pContainer)
{
    DIRECTION eHitPoint = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));

    if (eHitPoint == DIRECTION::F || eHitPoint == DIRECTION::FR || eHitPoint == DIRECTION::FL)
    {
        if (pContainer->IsHitPoint().IsHead || pContainer->IsHitPoint().IsBody)
            pContainer->Switch_Anim("Damage_F_Head", false);
        else if (pContainer->IsHitPoint().IsSholder_L)
            pContainer->Switch_Anim("Damage_F_Shoulder_R", false);
        else if (pContainer->IsHitPoint().isSholder_R)
            pContainer->Switch_Anim("Damage_F_Shoulder_L", false);
    }
    else if (eHitPoint == DIRECTION::B)
        pContainer->Switch_Anim("Damage_B_Body", false);
    else if (eHitPoint == DIRECTION::R || eHitPoint == DIRECTION::BR)
        pContainer->Switch_Anim("Damage_R_Body", false);
    else
        pContainer->Switch_Anim("Damage_L_Body", false);
}

void CDamage_Daniela::Big_Damage(CDaniela* pContainer)
{
    DIRECTION eHitPoint = PlayerDIR(pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_TransformState(STATE::LOOK));

    if (eHitPoint == DIRECTION::F || eHitPoint == DIRECTION::FR || eHitPoint == DIRECTION::FL)
    {
        if (pContainer->IsHitPoint().IsHead )
            pContainer->Switch_Anim("Damage_F_Head_Big", false);
        else if (pContainer->IsHitPoint().IsBody)
            pContainer->Switch_Anim("Damage_F_Body_Big", false);
        else if (pContainer->IsHitPoint().IsSholder_L)
            pContainer->Switch_Anim("Damage_FR_Body_Big", false);
        else if (pContainer->IsHitPoint().isSholder_R)
            pContainer->Switch_Anim("Damage_FL_Body_Big", false);

    }
    else if (eHitPoint == DIRECTION::B)
        pContainer->Switch_Anim("Damage_B_Body_Big", false);
    else if (eHitPoint == DIRECTION::R || eHitPoint == DIRECTION::BR)
        pContainer->Switch_Anim("Damage_R_Body_Big", false);
    else
        pContainer->Switch_Anim("Damage_L_Body_Big", false);
}


CDamage_Daniela* CDamage_Daniela::Create(void* pArg)
{
    CDamage_Daniela* pInstance = new CDamage_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDamage_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDamage_Daniela::Free()
{
    __super::Free();
}
