#include "pch.h"
#include "Die_Daniela.h"
#include "Daniela.h"

CDie_Daniela::CDie_Daniela()
{
}

HRESULT CDie_Daniela::Initalize(void* pArg)
{
    return S_OK;
}

void CDie_Daniela::Enter(CDaniela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    pContainer->Switch_AnimState(ENUM_CLASS(CDaniela::ANIM_STATE::DAMAGE));
    pContainer->Switch_Anim("Freezes_Die", false);
}

void CDie_Daniela::Update(CDaniela* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Target_LookTurn(fDeltatime);
        if (m_isNoies)
        {
            pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime * 0.5f;
            if (pContainer->Get_BlackBoard()->Get_Data().fNoies > 1.f)
            {
                pContainer->SetDead();
                m_pGameInstance->OnOff_Light(TEXT("Light_Daniela"), false);

            }
        }
        else if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_isNoies = true;
           
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
    }
}

void CDie_Daniela::Exit(CDaniela* pContainer)
{
}

CDie_Daniela* CDie_Daniela::Create(void* pArg)
{
    CDie_Daniela* pInstance = new CDie_Daniela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CDie_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDie_Daniela::Free()
{
    __super::Free();
}
