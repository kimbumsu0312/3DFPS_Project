#include "pch.h"
#include "Critical_Attack_Daniela.h"
#include "Daniela.h"

CCritical_Attack_Daniela::CCritical_Attack_Daniela()
{
}

HRESULT CCritical_Attack_Daniela::Initalize(void* pArg)
{
    return S_OK;
}

void CCritical_Attack_Daniela::Enter(CDaniela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CDaniela::ANIM_STATE::ATTACK);

    pContainer->Switch_Anim("Attack_CriticalHit", false);
}

void CCritical_Attack_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
    pContainer->Target_LookTurn(fDeltatime);

    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Attack_Collision();
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().fCriAttackCool = 15.f;
            pContainer->Get_BlackBoard()->Set_Data().fAttackCool = 3.f;
            pContainer->Get_BlackBoard()->Set_Data().IsCriticalAttack = false;
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {

    }

}

void CCritical_Attack_Daniela::Exit(CDaniela* pContainer)
{
}

CCritical_Attack_Daniela* CCritical_Attack_Daniela::Create(void* pArg)
{
    CCritical_Attack_Daniela* pInstance = new CCritical_Attack_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CCritical_Attack_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCritical_Attack_Daniela::Free()
{
    __super::Free();
}
