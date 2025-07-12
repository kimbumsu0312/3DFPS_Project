#include "pch.h"
#include "Item_Manager.h"
#include "Item.h"
#include "Client_Item.h"
#include "2DItem.h"
IMPLEMENT_SINGLETON(CItem_Manager)

CItem_Manager::CItem_Manager()
{
}

HRESULT CItem_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
	
	if (m_pGameInstance == nullptr)
		return E_FAIL;
	return S_OK;
}

HRESULT CItem_Manager::Add_2DRenderGroup(const ITEM_2DRENDER_TYPE& eRenderType, _uint ItemIndex, _float2 vPos)
{

	return E_NOTIMPL;
}

void CItem_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
