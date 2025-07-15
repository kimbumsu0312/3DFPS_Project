#include "pch.h"
#include "UI_Slot.h"

CUI_Slot::CUI_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CButton{ pDevice, pContext }
{
}

CUI_Slot::CUI_Slot(const CUI_Slot& Prototype) : CButton(Prototype)
{
}

HRESULT CUI_Slot::Initialize_Prototype()
{
	if (FAILED(Ready_Children_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Slot::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	UI_SLOT_DESC* Desc = static_cast<UI_SLOT_DESC*>(pArg);

	m_iTexIndex = Desc->iTexIndex;
	m_iMaxIndex = Desc->iMaxIndex;
	m_iIndex = Desc->iIndex;
	m_iPassIndex = Desc->iPassIndex;
	m_vLocalSize = Desc->vSize;

	_int iIndex = Desc->iIndex;

	if (m_iMaxIndex <= m_iIndex)
		iIndex = iIndex % m_iMaxIndex;
	
 	m_vLocalPos.x = Desc->vPos.x + iIndex * Desc->OffsetX - (Desc->iMaxIndex - 1) * Desc->OffsetX / 2;;;
	m_vLocalPos.y = Desc->vPos.y + Desc->OffsetX * Desc->OffsetY;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CUI_Slot::Priority_Update(_float fTimeDelta)
{

}

void CUI_Slot::Update(_float fTimeDelta)
{

}

void CUI_Slot::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
		return;
}

HRESULT CUI_Slot::Render()
{
	if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", m_iTexIndex)))
		return E_FAIL;

	__super::Bind_ShaderTransform_Resourc(m_iPassIndex);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Slot::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Slot::Ready_Children_Prototype()
{
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line"),
		CLogo_Button_Line::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CUI_Slot::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	//UIOBJECT_DESC Desc;
	//Desc.vSize = { 255.f, 18.f };
	//Desc.vPos = { 0, 20 };
	//Desc.vMinUV = { 500.f / 1024.f, 0.f };
	//Desc.vMaxUV = { 755.f / 1024.f, 18.f / 512.f };


	//pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line"), &Desc));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

	return S_OK;
}

CUI_Slot* CUI_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Slot* pInstance = new CUI_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Slot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Slot::Clone(void* pArg)
{
	CUI_Slot* pInstance = new CUI_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Slot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
}
