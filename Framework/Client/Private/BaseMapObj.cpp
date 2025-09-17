#include "pch.h"
#include "BaseMapObj.h"

CBaseMapObj::CBaseMapObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject(pDevice, pContext)
{
}

CBaseMapObj::CBaseMapObj(const CBaseMapObj& Prototype) : CGameObject(Prototype)
{
}

HRESULT CBaseMapObj::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBaseMapObj::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* Desc = static_cast<GAMEOBJECT_DESC*>(pArg);
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(Desc->szModel_Path)))
		return E_FAIL;

	return S_OK;
}

void CBaseMapObj::Priority_Update(_float fTimeDelta)
{
}

void CBaseMapObj::Update(_float fTimeDelta)
{
}

void CBaseMapObj::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;

	if(m_pGameInstance->Get_MapShadow() == true)
		if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::SHADOW, this)))
			return;
}

HRESULT CBaseMapObj::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
			continue;

		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, 1, 0)))
			m_pShaderCom->Begin(0);
		else
			m_pShaderCom->Begin(2);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

HRESULT CBaseMapObj::Render_Shadow()
{
	m_pGameInstance->On_Static_Shadow(false);
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_ShadowLight_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_ShadowLight_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(4);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CBaseMapObj::Ready_Components(_wstring szModelPath)
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szModelPath,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CBaseMapObj::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CBaseMapObj* CBaseMapObj::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBaseMapObj* pInstance = new CBaseMapObj(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CBaseMapObj"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaseMapObj::Clone(void* pArg)
{
	CBaseMapObj* pInstance = new CBaseMapObj(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBaseMapObj"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaseMapObj::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
