#include "pch.h"
#include "Walk_Bela.h"
#include "Bela.h"

CWalk_Bela::CWalk_Bela()
{
}

HRESULT CWalk_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CWalk_Bela::Enter(CBela* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().bIsFly = true;
    m_fSpeed = pContainer->Get_BlackBoard()->Get_Data().fMoveSpeed;
    pContainer->SetUp_Node();
}

void CWalk_Bela::Update(CBela* pContainer, _float fTimeDelta)
{
    pContainer->Target_LookTurn_Navi(fTimeDelta);
    pContainer->Get_Transform()->Go_Straight(fTimeDelta* m_fSpeed);

    _vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
    _vector vMonPos = pContainer->Get_Transform()->Get_State(STATE::POSITION);
    _float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMonPos));

    if (fDis < 0.5f && fDis > -0.5f)
    {
        pContainer->Get_BlackBoard()->Set_Data().IsEvent_1 = true;
    }
}
void CWalk_Bela::Exit(CBela* pContainer)
{
}

CWalk_Bela* CWalk_Bela::Create(void* pArg)
{
    CWalk_Bela* pInstance = new CWalk_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CWalk_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWalk_Bela::Free()
{
    __super::Free();
}
