#include "pch.h"
#include "Event1_Alcina.h"

CEvent1_Alcina::CEvent1_Alcina()
{
}

HRESULT CEvent1_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CEvent1_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::NORMAL);
    pContainer->Switch_Anim("FlyBall", true);

    CFly_Effect::FLY_EFFECT_INIT Desc;
    Desc.vPos = pContainer->Get_TransForm()->Get_State(STATE::POSITION);
    Desc.vPos = XMVectorSetY(Desc.vPos, XMVectorGetY(Desc.vPos) + 1.5f);
    m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Fly_Spread"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &Desc);
    pContainer->Get_BlackBoard()->Set_Data().bIsFly = true;
}

void CEvent1_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP;
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Get_BlackBoard()->Set_Data().fNoies += fDeltatime;
        if(pContainer->Get_BlackBoard()->Get_Data().fNoies >= 1.f)
            pContainer->Get_BlackBoard()->Set_Data().IsEvent_1 = false;
    }
}

void CEvent1_Alcina::Exit(CAlcina* pContainer)
{
}

CEvent1_Alcina* CEvent1_Alcina::Create(void* pArg)
{
    CEvent1_Alcina* pInstance = new CEvent1_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CEvent1_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEvent1_Alcina::Free()
{
    __super::Free();
}
