#include "pch.h"
#include "Guard_Player.h"
#include "Player.h"

CGuard_Player::CGuard_Player()
{
}

HRESULT CGuard_Player::Initalize(void* pArg)
{
    return S_OK;
}

void CGuard_Player::Enter(CPlayer* pContainer)
{
    pContainer->Switch_Anim("Guard_Loop", true);
}

void CGuard_Player::Update(CPlayer* pContainer, _float fTimeDelta)
{
  if (pContainer->Get_BlackBoard()->Get_Data().isMove == true)
      pContainer->Switch_Anim("Guard_Walk", true);
  else
      pContainer->Switch_Anim("Guard_Loop", true);
}

void CGuard_Player::Exit(CPlayer* pContainer)
{
}

CGuard_Player* CGuard_Player::Create(void* pArg)
{
    CGuard_Player* pInstance = new CGuard_Player();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CGuard_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGuard_Player::Free()
{
    __super::Free();
}
