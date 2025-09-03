#include "pch.h"
#include "Die_NorMon_1.h"
#include "Monster_Normal.h"
#include "ItemSpawner.h"
CDie_Normon_1::CDie_Normon_1()
{
}

HRESULT CDie_Normon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_Normon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::DAMAGE);
    pContainer->Switch_Anim("Die", false);
}

void CDie_Normon_1::Update(CMonster_Normal* pContainer, _float fDeltatime)
{
    if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
    {
        pContainer->SetDead();
        _int i = rand() % 4 + 5;
        
        CItemSpawner::GetInstance()->Spawn_Item(i, pContainer->Get_TransformState(STATE::POSITION), pContainer->Get_CulNaviIndex());
    }
}

void CDie_Normon_1::Exit(CMonster_Normal* pContainer)
{
}

CDie_Normon_1* CDie_Normon_1::Create(void* pArg)
{
    CDie_Normon_1* pInstance = new CDie_Normon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_Normon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_Normon_1::Free()
{
    __super::Free();
}
