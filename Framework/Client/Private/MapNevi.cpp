#include "pch.h"
#include "MapNevi.h"

CMapNevi::CMapNevi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject{ pDevice, pContext }
{
}

CMapNevi::CMapNevi(const CMapNevi& Prototype) : CGameObject(Prototype)
{
}

HRESULT CMapNevi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapNevi::Initialize(void* pArg)
{

	__super::Initialize(pArg);
	Ready_Components();

	return S_OK;
}

void CMapNevi::Priority_Update(_float fTimeDelta)
{
}

void CMapNevi::Update(_float fTimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CMapNevi::Late_Update(_float fTimeDelta)
{
#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
}

HRESULT CMapNevi::Render()
{
	return S_OK;
}

HRESULT CMapNevi::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;
	
#ifdef _DEBUG
	_float4 vColor = { 0.f, 1.f, 0.f, 1.f };
	m_pNavigationCom->Chage_Color(vColor);
#endif
	return S_OK;
}

CMapNevi* CMapNevi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMapNevi* pInstance = new CMapNevi(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CMapNevi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapNevi::Clone(void* pArg)
{
	CMapNevi* pInstance = new CMapNevi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMapNevi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapNevi::Free()
{
	__super::Free();
	Safe_Release(m_pNavigationCom);
}
