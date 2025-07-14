#include "pch.h"
#include "2DItem.h"

C2DItem::C2DItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CItem{pDevice, pContext}
{
}

C2DItem::C2DItem(const C2DItem& Prototype) : CItem(Prototype)
{
}

HRESULT C2DItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT C2DItem::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	ITEM_2D_DESC* pDesc = static_cast<ITEM_2D_DESC*>(pArg);
	if (pArg != nullptr)
	{
		m_vMinUV = pDesc->vMinUV;
		m_vMaxUV = pDesc->vMaxUV;
	}
	m_vSize = m_vLocalSize;

	D3D11_VIEWPORT			Viewport{};

	_uint			iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.0f, 1.f));

	return S_OK;
}

void C2DItem::Priority_Update(_float fTimeDelta)
{
}

void C2DItem::Update(_float fTimeDelta)
{
}

void C2DItem::Late_Update(_float fTimeDelta)
{
}

HRESULT C2DItem::Bind_Transform_Resourc(class CShader* pShader)
{
	m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
	m_pTransformCom->Bind_Shader_Resource(pShader, "g_WorldMatrix");

	return S_OK;
}
HRESULT C2DItem::Render()
{
	return S_OK;
}

HRESULT C2DItem::Item_RenderSet(void* pArg)
{
	return E_NOTIMPL;
}

void C2DItem::Free()
{
	__super::Free();
}
