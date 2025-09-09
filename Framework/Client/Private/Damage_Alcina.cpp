#include "pch.h"
#include "Damage_Alcina.h"

CDamage_Alcina::CDamage_Alcina()
{
}

HRESULT CDamage_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CDamage_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::DAMAGE);

    DIRECTION eDir = PlayerDIR(pContainer->Get_TransForm()->Get_State(STATE::POSITION), pContainer->Get_TransForm()->Get_State(STATE::LOOK));

    if (eDir == DIRECTION::B || eDir == DIRECTION::BL || eDir == DIRECTION::BR)
        pContainer->Switch_Anim("Damage_Turn_B", false);
    else if (eDir == DIRECTION::L)
        pContainer->Switch_Anim("Damage_Turn_L", false);
    else if (eDir == DIRECTION::R)
        pContainer->Switch_Anim("Damage_Turn_R", false);
    else
        Damage_F(pContainer);
}

void CDamage_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        pContainer->Get_BlackBoard()->Set_Data().iDamage = 0;
}

void CDamage_Alcina::Exit(CAlcina* pContainer)
{
}

void CDamage_Alcina::Damage_F(CAlcina* pContainer)
{
    Damage_Type eDamageType;

    _int iDamage = pContainer->Get_BlackBoard()->Get_Data().iDamage;
    if (iDamage < 100)
        eDamageType = Damage_Type::LOW;
    else if (iDamage < 150)
        eDamageType = Damage_Type::MIDDIE;
    else
        eDamageType = Damage_Type::HIGHT;

    if (pContainer->Get_HeadShot())
    {
        switch (eDamageType)
        {
        case Damage_Type::LOW:
            pContainer->Switch_Anim("Damage_Head_Low", false);
            break;
        case Damage_Type::MIDDIE:
            pContainer->Switch_Anim("Damage_Head_Middie", false);
            break;
        case Damage_Type::HIGHT:
            pContainer->Switch_Anim("Damage_Head_High", false);
            break;
        }

    }
    else
    {
        switch (eDamageType)
        {
        case Damage_Type::LOW:
            pContainer->Switch_Anim("Damage_Upper_Body_Low", false);
            break;
        case Damage_Type::MIDDIE:
            pContainer->Switch_Anim("Damage_Upper_Body_Meddie", false);
            break;
        case Damage_Type::HIGHT:
            pContainer->Switch_Anim("Damage_Upper_Body_Hight", false);
            break;
        }
    }
}

CDamage_Alcina* CDamage_Alcina::Create(void* pArg)
{
    CDamage_Alcina* pInstance = new CDamage_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDamage_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDamage_Alcina::Free()
{
    __super::Free();
}
