#include "pch.h"
#include "UIObject.h"
#include "Shader.h"

CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject{ pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject& Prototype) : CGameObject(Prototype)
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_vLocalPos.x = pDesc->fX;
	m_vLocalPos.y = pDesc->fY;
	m_vLocalSize.x = pDesc->fSizeX;
	m_vLocalSize.y = pDesc->fSizeY;

	m_vSize.x = pDesc->fSizeX;
	m_vSize.y = pDesc->fSizeY;

	D3D11_VIEWPORT			Viewport{};

	_uint			iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.0f, 1.f));

	m_iWinSizeX = Viewport.Width;
	m_iWinSizeY = Viewport.Height;

	Update_Position();

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
	return S_OK;
}

HRESULT CUIObject::Bind_Shader_Resourec(CShader* pShader)
{
	m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
 	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - m_iWinSizeX * 0.5f, -m_vPos.y + m_iWinSizeY * 0.5f, 0.0f, 1.0f));

	m_pTransformCom->Bind_Shader_Resource(pShader, "g_WorldMatrix");
	
	if (FAILED(pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	pShader->Begin(0);

	return S_OK;
}

_bool CUIObject::IsPick()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rcUI = { m_vPos.x - m_vSize.x * 0.5f, m_vPos.y - m_vSize.y * 0.5f, m_vPos.x + m_vSize.x * 0.5f, m_vPos.y + m_vSize.y * 0.5f };

	return PtInRect(&rcUI, ptMouse);
}

void CUIObject::Update_Position()
{
	if (m_pParent != nullptr)
	{
		m_vPos.x = m_pParent->m_vPos.x + m_vLocalPos.x;
		m_vPos.y = m_pParent->m_vPos.y + m_vLocalPos.y;
		m_vPos.z = m_pParent->m_vPos.z;
	}
	else
	{
		m_vPos.x = m_vLocalPos.x;
		m_vPos.y = m_vLocalPos.y;
		m_vPos.z = m_vLocalPos.z;
	}
	
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - m_iWinSizeX * 0.5f, -m_vPos.y + m_iWinSizeY * 0.5f, m_vPos.z, 1.f));

}

CGameObject* CUIObject::Clone(void* pArg)
{
	return nullptr;
}

void CUIObject::Free()
{
	__super::Free();
}
