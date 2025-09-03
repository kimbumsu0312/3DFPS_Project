#include "pch.h"
#include "Attack_NorMon_1.h"
#include "Monster_Normal.h"

CAttack_NorMon_1::CAttack_NorMon_1()
{
}

HRESULT CAttack_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    _int Rand = rand() % 3;
    m_eAnimState = STATE_ANIM::START;
    
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::ATTACK);
    
    if (pContainer->Get_BlackBoard()->Get_Data().iWeapon == ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END))
    {
        switch (Rand)
        {
        case 0:
            pContainer->Switch_Anim("Grapple_Attack_1", false);
            break;
        case 1:
            pContainer->Switch_Anim("Grapple_Attack_2", false);
            break;
        case 2:
            pContainer->Switch_Anim("Grapple_Attack_3", false);
            break;
        }
    }
    else
    {
        switch (Rand)
        {
        case 0:
            pContainer->Switch_Anim("Sword_Attack_1", false);
            break;
        case 1:
            pContainer->Switch_Anim("Sword_Attack_2", false);
            break;
        case 2:
            pContainer->Switch_Anim("Sword_Attack_3", false);
            break;
        }
    }
}

void CAttack_NorMon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    pContainer->Attack_Collision();
}

void CAttack_NorMon_1::Exit(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}

CAttack_NorMon_1* CAttack_NorMon_1::Create(void* pArg)
{
    CAttack_NorMon_1* pInstance = new CAttack_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack_NorMon_1::Free()
{
    __super::Free();
}
