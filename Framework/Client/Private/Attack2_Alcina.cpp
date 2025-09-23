#include "pch.h"
#include "Attack2_Alcina.h"

CAttack2_Alcina::CAttack2_Alcina()
{
}

HRESULT CAttack2_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack2_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::ATTACK);
    pContainer->Switch_Anim("Claw_Moving_Attack_Start", false);
}

void CAttack2_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    pContainer->Target_LookTurn(fDeltatime);
    if (m_eAnimState == STATE_ANIM::START)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP;
            pContainer->Switch_Anim("Claw_Moving_Attack_Loop", false);
        }
    }
    else if(m_eAnimState == STATE_ANIM::LOOP)
    {
        _vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
        _vector vMonPos = pContainer->Get_TransForm()->Get_State(STATE::POSITION);
        _float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true || fDis <= 3.f)
        {
            m_eAnimState = STATE_ANIM::LOOP2;
            pContainer->Switch_Anim("Claw_Moving_Attack_End_Start", false);
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP2)
    {
        pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::LOOP3;
            pContainer->Switch_Anim("Claw_Moving_Attack_End_Attack", false);
        }
    }
    else if (m_eAnimState == STATE_ANIM::LOOP3)
    {
        CSpark_Effect::SPARK_EFFECT_INIT SparkDesc;

        pContainer->Attack_Collision();
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_PinkyNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_MiddleNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::END;
            pContainer->Switch_Anim("Claw_Moving_Attack_End_End", false);
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
    }
}

void CAttack2_Alcina::Exit(CAlcina* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;

    pContainer->Get_BlackBoard()->Set_Data().fAttack2Cool = 20.f;
}

CAttack2_Alcina* CAttack2_Alcina::Create(void* pArg)
{
    CAttack2_Alcina* pInstance = new CAttack2_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack2_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack2_Alcina::Free()
{
    __super::Free();
}
