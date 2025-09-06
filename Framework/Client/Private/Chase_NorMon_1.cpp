#include "pch.h"
#include "Chase_NorMon_1.h"
#include "Player_Manager.h"
#include "Monster_Normal.h"

CChase_NorMon_1::CChase_NorMon_1()
{
}

HRESULT CChase_NorMon_1::Initalize(void* pArg)
{
    return S_OK;
}

void CChase_NorMon_1::Enter(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::ATTACK);
    m_iWeapon = pContainer->Get_BlackBoard()->Get_Data().iWeapon;
    if (m_iWeapon == ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END))
        pContainer->Switch_Anim("Grapple_Walk_Start", false);
    else
        pContainer->Switch_Anim("Sword_Walk_Start", false);

    //_float3 vPlayerPos{};
    //XMStoreFloat3(&vPlayerPos, CPlayer_Manager::GetInstance()->Get_PlayerPos());
    //_vector vMonPos = { pContainer->Get_TransformState(STATE::POSITION)};
    //_float fDis = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPlayerPos) - vMonPos));
    //if (fDis >= 5.f)
    //{
    //    pContainer->SetUp_Node(CPlayer_Manager::GetInstance()->Get_CellIndex(), vPlayerPos);
    //}

}

void CChase_NorMon_1::Update(CMonster_Normal* pContainer, _float fTimeDelta)
{
    //pContainer->Move_Node(fTimeDelta);
    pContainer->Target_LookAt(fTimeDelta);
    if (m_eAnimState == STATE_ANIM::START)
    {
        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            if (m_iWeapon == ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END))
                pContainer->Switch_Anim("Grapple_Walk_Loop", true);
            else
                pContainer->Switch_Anim("Sword_Walk_Loop", true);
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {

    }
}

void CChase_NorMon_1::Exit(CMonster_Normal* pContainer)
{
    m_eAnimState = STATE_ANIM::END;
}

CChase_NorMon_1* CChase_NorMon_1::Create(void* pArg)
{
    CChase_NorMon_1* pInstance = new CChase_NorMon_1();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CChase_NorMon_1"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChase_NorMon_1::Free()
{
    __super::Free();
}
