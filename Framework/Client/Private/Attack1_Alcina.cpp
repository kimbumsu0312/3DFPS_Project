#include "pch.h"
#include "Attack1_Alcina.h"

CAttack1_Alcina::CAttack1_Alcina()
{
}

HRESULT CAttack1_Alcina::Initalize(void* pArg)
{
    return S_OK;
}

void CAttack1_Alcina::Enter(CAlcina* pContainer)
{
    m_eAnimState = STATE_ANIM::START;

    DIRECTION eDir = PlayerDIR(pContainer->Get_TransForm()->Get_State(STATE::POSITION), pContainer->Get_TransForm()->Get_State(STATE::LOOK));
    
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CAlcina::ANIM_STATE::ATTACK);
    if (eDir == DIRECTION::B || eDir == DIRECTION::BL || eDir == DIRECTION::BR)
    {
        pContainer->Switch_Anim("Claw_Behind_Start", false);
        m_szAttackName = TEXT("Claw_Behind_Start");
    }
    else if (m_iAttackCount == 0)
    {
        pContainer->Switch_Anim("Claw_Left_Zero_Start", false);
        m_iAttackCount++;
        m_szAttackName = TEXT("Claw_Left_Zero_Start");
    }
    else if(m_iAttackCount == 1)
    {
        pContainer->Switch_Anim("Claw_Left_Short_Start", false);
        m_iAttackCount++;
        m_szAttackName = TEXT("Claw_Left_Short_Start");
    }
    else if (m_iAttackCount == 2)
    {
        pContainer->Switch_Anim("Claw_Right_Short_Start", false);
        m_iAttackCount = 0;
        m_szAttackName = TEXT("Claw_Right_Short_Start");
    }
}

void CAttack1_Alcina::Update(CAlcina* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            pContainer->Get_BlackBoard()->Set_Data().isBogan = false;
            if (m_szAttackName == TEXT("Claw_Behind_Start"))
            {
                pContainer->Trail_On(false);
                pContainer->Switch_Anim("Claw_Behind_Attack", false);
                m_szAttackName = TEXT("Claw_Behind_Attack");
            }
            else if (m_szAttackName == TEXT("Claw_Left_Zero_Start"))
            {
                pContainer->Trail_On(false);
                pContainer->Switch_Anim("Claw_Left_Zero_Attack", false);
                m_szAttackName = TEXT("Claw_Left_Zero_Attack");
            }
            else if (m_szAttackName == TEXT("Claw_Left_Short_Start"))
            {
                pContainer->Trail_On(false);
                pContainer->Switch_Anim("Claw_Left_Short_Attack", false);
                m_szAttackName = TEXT("Claw_Left_Short_Attack");
            }
            else if (m_szAttackName == TEXT("Claw_Right_Short_Start"))
            {
                pContainer->Trail_On(true);
                pContainer->Switch_Anim("Claw_Right_Short_Attack", false);
                m_szAttackName = TEXT("Claw_Right_Short_Attack");

            }
 
            m_eAnimState = STATE_ANIM::LOOP2;
        }        
    }
    else if (m_eAnimState == STATE_ANIM::LOOP2)
    {
        pContainer->Attack_Collision();
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            if (m_szAttackName == TEXT("Claw_Behind_Attack"))
            {
                pContainer->Switch_Anim("Claw_Behind_End", false);
                m_szAttackName = TEXT("Claw_Behind_End");
            }
            else if (m_szAttackName == TEXT("Claw_Left_Zero_Attack"))
            {
                pContainer->Switch_Anim("Claw_Left_Zero_End", false);
                m_szAttackName = TEXT("Claw_Left_Zero_End");
            }
            else if (m_szAttackName == TEXT("Claw_Left_Short_Attack"))
            {
                pContainer->Switch_Anim("Claw_Left_Short_End", false);
                m_szAttackName = TEXT("Claw_Left_Short_End");
            }
            else if (m_szAttackName == TEXT("Claw_Right_Short_Attack"))
            {
                pContainer->Switch_Anim("Claw_Right_Short_End", false);
                m_szAttackName = TEXT("Claw_Right_Short_End");

            }
            m_eAnimState = STATE_ANIM::END;
            pContainer->Trail_Off();
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
            pContainer->Get_BlackBoard()->Set_Data().IsAttack = false;
    }
}

void CAttack1_Alcina::Exit(CAlcina* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().isBogan = true;

}

void CAttack1_Alcina::Attack_Effect(CAlcina* pContainer)
{
    CSpark_Effect::SPARK_EFFECT_INIT SparkDesc;

    if (m_szAttackName == TEXT("Claw_Behind_Attack"))
    {
        pContainer->Attack_Collision();
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_PinkyNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_RingNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_MiddleNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
    }
    else if (m_szAttackName == TEXT("Claw_Left_Zero_Attack"))
    {
        pContainer->Attack_Collision();
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_PinkyNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_RingNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_MiddleNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
    }
    else if (m_szAttackName == TEXT("Claw_Left_Short_Attack"))
    {
        pContainer->Attack_Collision();
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_PinkyNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_RingNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_MiddleNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("L_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
    }
    else if (m_szAttackName == TEXT("Claw_Right_Short_Attack"))
    {
        pContainer->Attack_Collision();
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_PinkyNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_MiddleNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
        SparkDesc.vPos = pContainer->Bone_WorldTransform(TEXT("R_IndexNail_1"));
        m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);
    }
}

CAttack1_Alcina* CAttack1_Alcina::Create(void* pArg)
{
    CAttack1_Alcina* pInstance = new CAttack1_Alcina();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAttack1_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack1_Alcina::Free()
{
    __super::Free();
}
