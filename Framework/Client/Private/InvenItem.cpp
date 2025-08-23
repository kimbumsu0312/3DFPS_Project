#include "pch.h"
#include "InvenItem.h"

CInvenItem::CInvenItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolUIObj{ pDevice, pContext }
{
}

CInvenItem::CInvenItem(const CInvenItem& Prototype) : CPoolUIObj(Prototype)
{
}

HRESULT CInvenItem::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CInvenItem::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Children()))
		return E_FAIL;
	return S_OK;
}

void CInvenItem::Priority_Update(_float fTimeDelta)
{
}

void CInvenItem::Update(_float fTimeDelta)
{
}

void CInvenItem::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
		return;

}

HRESULT CInvenItem::Render()
{
	Bind_ShaderTransform_Resourc(2);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CInvenItem::Initialize_Pool(void* pArg)
{
	ITEM_DESC* pDesc = static_cast<ITEM_DESC*>(pArg);
	m_ItemData = pDesc->ItemData;

	__super::Initialize_Pool(pArg);
	return S_OK;
}

void CInvenItem::Return_Pool()
{
}

HRESULT CInvenItem::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenItem::Ready_Children()
{
	return S_OK;
}

CInvenItem* CInvenItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInvenItem* pInstance = new CInvenItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CInvenItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInvenItem::Clone(void* pArg)
{
	CInvenItem* pInstance = new CInvenItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInvenItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvenItem::Free()
{
	__super::Free();
}
