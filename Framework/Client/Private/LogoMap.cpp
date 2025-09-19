#include "pch.h"
#include "LogoMap.h"

CLogoMap::CLogoMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject(pDevice, pContext)
{
}

CLogoMap::CLogoMap(const CLogoMap& Prototype) : CGameObject(Prototype)
{
}

HRESULT CLogoMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogoMap::Initialize(void* pArg)
{
	LOGOOBJECT_DESC* Desc = static_cast<LOGOOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(Desc->szModel_Path)))
		return E_FAIL;

	m_pTransformCom->Scale(Desc->vScale);
	m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&Desc->vPos));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(Desc->fDegrees));
	return S_OK;
}

void CLogoMap::Priority_Update(_float fTimeDelta)
{
}

void CLogoMap::Update(_float fTimeDelta)
{
}

void CLogoMap::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	if (m_pGameInstance->Get_MapShadow() == true)
		if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::SHADOW, this)))
			return;
}

HRESULT CLogoMap::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
			continue;
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, 1, 0)))
			continue;
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_AtosTexture", i, 2, 0)))
			continue;

		m_pShaderCom->Begin(5);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

HRESULT CLogoMap::Render_Shadow()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_ShadowLight_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_ShadowLight_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(5);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CLogoMap::Ready_Components(_wstring szModelPath)
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::LOGO), szModelPath,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogoMap::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CLogoMap* CLogoMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLogoMap* pInstance = new CLogoMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CLogoMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLogoMap::Clone(void* pArg)
{
	CLogoMap* pInstance = new CLogoMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CLogoMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogoMap::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
