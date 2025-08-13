#include "pch.h"
#include "PointCube.h"

CPointCube::CPointCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CPointCube::CPointCube(const CPointCube& Prototype) : CPartObject(Prototype)
{
}

void CPointCube::Set_Transform(_float3 vPos)
{
	 m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPos), 1.f)); 
}

HRESULT CPointCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPointCube::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scale(_float3{ 0.1f, 0.1f, 0.1f });
	return S_OK;
}

void CPointCube::Priority_Update(_float fTimeDelta)
{
}

void CPointCube::Update(_float fTimeDelta)
{


}

void CPointCube::Late_Update(_float fTimeDelta)
{
	Update_CombinedMatrix();
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;
}

HRESULT CPointCube::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if(m_bIsSelete)
		m_pShaderCom->Begin(1);
	else
		m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Resources();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPointCube::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPointCube::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CPointCube* CPointCube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPointCube* pInstance = new CPointCube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPointCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPointCube::Clone(void* pArg)
{
	CPointCube* pInstance = new CPointCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CPointCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPointCube::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
