#include "pch.h"
#include "UIObject.h"
#include "Shader.h"
#include "Texture.h"
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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);
		m_vMinUV = pDesc->vMinUV;
		m_vMaxUV = pDesc->vMaxUV;
	}
	m_vSize = m_vLocalSize;
	
	D3D11_VIEWPORT			Viewport{};

	_uint			iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.0f, 1.f));

	Update_Position();

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
	for (auto child : m_vecChildren)
	{
		if (m_vecChildren.size() == 0)
			return;
		if (child != nullptr)
			child->Priority_Update(fTimeDelta);
	}
}

void CUIObject::Update(_float fTimeDelta)
{
	for (auto child : m_vecChildren)
	{
		if (m_vecChildren.size() == 0)
			return;
		if (child != nullptr)
			child->Update(fTimeDelta);
	}
}

void CUIObject::Late_Update(_float fTimeDelta)
{
	for (auto child : m_vecChildren)
	{
		if (m_vecChildren.size() == 0)
			return;
		if (child != nullptr)
			child->Late_Update(fTimeDelta);
	}
}

HRESULT CUIObject::Render()
{
	return S_OK;
}

HRESULT CUIObject::Bind_ShaderTransform_Resourc(_uint iPassIndex)
{
	m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
	m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
		return E_FAIL;

	m_pShaderCom->Begin(iPassIndex);

	return S_OK;
}

void CUIObject::Update_Position(CUIObject* pParent)
{
	if (pParent != nullptr)
	{
		m_vPos.x = pParent->m_vPos.x + m_vLocalPos.x;
		m_vPos.y = pParent->m_vPos.y + m_vLocalPos.y;
	}
	else
	{
		m_vPos = m_vLocalPos;
	}

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	for (auto child : m_vecChildren)
		child->Update_Position(this);
}

void CUIObject::Update_Position_Children(CUIObject* pParent)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	for (auto child : m_vecChildren)
		child->Update_Position(this);
}

void CUIObject::Add_Child(CUIObject* pParent, CUIObject* pChild, CShader* pShader, CTexture* pTexture)
{
	m_vecChildren.push_back(pChild);
	pChild->Update_Position(pParent);
	pChild->Update_Shader(pShader);
	pChild->Update_Texture(pTexture);
}

void CUIObject::Update_Shader(CShader* pShader)
{
	m_pShaderCom = pShader;
	Safe_AddRef(m_pShaderCom);

	for (auto child : m_vecChildren)
		child->Update_Shader(pShader);
}

void CUIObject::Update_Texture(CTexture* pTexture)
{
	m_pTextureCom = pTexture;
	Safe_AddRef(m_pTextureCom);

	for (auto child : m_vecChildren)
		child->Update_Texture(pTexture);
}

void CUIObject::Correct_Position(_float2 pPos)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(pPos.x - g_iWinSizeX * 0.5f, -pPos.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CUIObject::Free()
{
	
	for (auto& pChildren : m_vecChildren)
		Safe_Release(pChildren);
	m_vecChildren.clear();

	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
