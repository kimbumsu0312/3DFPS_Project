#include "pch.h"
#include "MapObject.h"
#include "Edit_Model.h"
#include "Imgui_Manager.h"
CMapObject::CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject(pDevice, pContext)
{
}

CMapObject::CMapObject(const CMapObject& Prototype) : CGameObject (Prototype)
{
}

HRESULT CMapObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (pArg != nullptr)
	{
		MODEL_OBJECT_DESC* pDesc = static_cast<MODEL_OBJECT_DESC*>(pArg);
		m_szModelPath = pDesc->szModelPath;
	}
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		MODEL_OBJECT_DESC* pDesc = static_cast<MODEL_OBJECT_DESC*>(pArg);
		m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&(pDesc->vPos)));
	}

	return S_OK;
}

void CMapObject::Priority_Update(_float fTimeDelta)
{
}

void CMapObject::Update(_float fTimeDelta)
{
	if (g_SeleteModel)
	{
		if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::LB))
		{
			_float3 vSetModelPos = { 0.f, 0.f, 0.f };
			if (m_pModelCom->Selete_Model(*m_pTransformCom, vSetModelPos))
			{
				CImgui_Manger::GetInstance()->Selete_Object(*this, *m_pTransformCom);
			}
		}
	}


}

void CMapObject::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;
}

HRESULT CMapObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

HRESULT CMapObject::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), m_szModelPath,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CMapObject* CMapObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMapObject* pInstance = new CMapObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CMapObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapObject::Clone(void* pArg)
{
	CMapObject* pInstance = new CMapObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMapObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
