#include "pch.h"
#include "SpawnPoint.h"

CSpawnPoint::CSpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CSpawnPoint::CSpawnPoint(const CSpawnPoint& Prototype) : CPartObject(Prototype)
{
}

HRESULT CSpawnPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpawnPoint::Initialize(void* pArg)
{
	SPAWNPOINTDESC* pDesc = static_cast<SPAWNPOINTDESC*>(pArg);
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scale(_float3{ 0.1f, 0.1f, 0.1f });
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPostion, 1.f));
	return S_OK;
}

void CSpawnPoint::Priority_Update(_float fTimeDelta)
{
}

void CSpawnPoint::Update(_float fTimeDelta)
{


}

void CSpawnPoint::Late_Update(_float fTimeDelta)
{
 	Update_CombinedMatrix();
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;
}

HRESULT CSpawnPoint::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_Resources();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSpawnPoint::Ready_Components()
{
 	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawnPoint::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CSpawnPoint* CSpawnPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpawnPoint* pInstance = new CSpawnPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSpawnPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawnPoint::Clone(void* pArg)
{
	CSpawnPoint* pInstance = new CSpawnPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CSpawnPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawnPoint::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
