#include "pch.h"
#include "Model_Door.h"

CModel_Door::CModel_Door(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CModel_Door::CModel_Door(const CModel_Door& Prototype) : CPartObject(Prototype)
{
}

HRESULT CModel_Door::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel_Door::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });
	m_pTransformCom->Scale({ 1.1f , 1.07f , 1.f });
	return S_OK;
}

void CModel_Door::Priority_Update(_float fTimeDelta)
{
}

void CModel_Door::Update(_float fTimeDelta)
{

}

void CModel_Door::Late_Update(_float fTimeDelta)
{
	Update_CombinedMatrix();
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	if (m_pGameInstance->Get_MapShadow() == true)
		if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::SHADOW, this)))
			return;
}

HRESULT CModel_Door::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
			continue;

		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, 1, 0)))
			continue;
		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CModel_Door::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Door"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel_Door::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CModel_Door* CModel_Door::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CModel_Door* pInstance = new CModel_Door(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CModel_Door"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CModel_Door::Clone(void* pArg)
{
	CModel_Door* pInstance = new CModel_Door(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel_Door"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_Door::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
