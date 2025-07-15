#include "pch.h"
#include "Mouse_Click_Fx.h"

CMouse_Click_Fx::CMouse_Click_Fx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingObject{pDevice, pContext}
{
}

CMouse_Click_Fx::CMouse_Click_Fx(const CMouse_Click_Fx& Prototype) : CPoolingObject(Prototype)
{
}

HRESULT CMouse_Click_Fx::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMouse_Click_Fx::Initialize(void* pArg)
{
  	CLICKFX_CLONE_DESC* pDesc = static_cast<CLICKFX_CLONE_DESC*>(pArg);
	
	m_vMinUV = pDesc->vMinUV;
	m_vMaxUV = pDesc->vMaxUV;
	m_fAlpha = 1.f;
	m_fAccScale = 1.f;
	m_vSize = pDesc->vSize;
	m_fAlphaTime = 1.5f;
	m_fScaleTime = 2.f;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

 	if (FAILED(Ready_Components()))
		return E_FAIL;


	D3D11_VIEWPORT			Viewport{};

	_uint			iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.0f, 1.f));

	return S_OK;
}

void CMouse_Click_Fx::Priority_Update(_float fTimeDelta)
{
}

void CMouse_Click_Fx::Update(_float fTimeDelta)
{
	m_fAlpha -= fTimeDelta * m_fAlphaTime;
	m_fAccScale += fTimeDelta * m_fScaleTime;

	if (m_fAlpha <= 0.f)
		m_bIsDead = true;

	
}

void CMouse_Click_Fx::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI_EFFECT, this)))
		return;
}

HRESULT CMouse_Click_Fx::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4 vVector = { 0.f, g_iWinSizeX, 0.f, g_iWinSizeY };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &vVector, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pTransformCom->Scale(_float3(m_vSize.x * m_fAccScale, m_vSize.y * m_fAccScale, 1.f));
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
	m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");

	if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

void CMouse_Click_Fx::On_Dead()
{
	__super::On_Dead();
}

HRESULT CMouse_Click_Fx::Initialize_Pool(void* pArg)
{
	CLICKFX_POOL_DESC* pDesc = static_cast<CLICKFX_POOL_DESC*>(pArg);

	m_vPos = pDesc->vPos;

	return S_OK;
}

void CMouse_Click_Fx::Return_Pool()
{
	m_fAlpha = 1.f;
	m_fAccScale = 1.f;
	m_bIsDead = false;
}

HRESULT CMouse_Click_Fx::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Text_Mouse"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

CMouse_Click_Fx* CMouse_Click_Fx::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMouse_Click_Fx* pInstance = new CMouse_Click_Fx(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CMouse_Click_Fx"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMouse_Click_Fx::Clone(void* pArg)
{
	CMouse_Click_Fx* pInstance = new CMouse_Click_Fx(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMouse_Click_Fx"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMouse_Click_Fx::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
