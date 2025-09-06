#include "pch.h"
#include "BehaviorTree_Alcina.h"
#include "Player_Manager.h"
#include "BehaviorTree_WereWolf.h"

CBehaviorTree_WereWolf::CBehaviorTree_WereWolf()
{
}

HRESULT CBehaviorTree_WereWolf::Initalize(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData)
{
	if (pData == nullptr)
		return E_FAIL;
	m_pBlackBoard = pData;
	Safe_AddRef(m_pBlackBoard);

	//if (FAILED(Ready_Node()))
	//	return E_FAIL;

	return S_OK;
}

void CBehaviorTree_WereWolf::Update()
{
	__super::Update();
}

CBehaviorTree_WereWolf* CBehaviorTree_WereWolf::Create(CBlackBoard<CMonster_WereWolf::WereWolfData>* pData)
{
	CBehaviorTree_WereWolf* pInstance = new CBehaviorTree_WereWolf();

	if (FAILED(pInstance->Initalize(pData)))
	{
		MSG_BOX(TEXT("Failed to Create : CBehaviorTree_WereWolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBehaviorTree_WereWolf::Free()
{
	__super::Free();

	Safe_Release(m_pBlackBoard);
}
