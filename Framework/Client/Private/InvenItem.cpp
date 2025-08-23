#include "pch.h"
#include "InvenItem.h"
#include "Item_Slot.h"
#include "Inven_Manager.h"
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
	m_pItemSlot->Priority_Update(fTimeDelta);
}

void CInvenItem::Update(_float fTimeDelta)
{
	m_pItemSlot->Update(fTimeDelta, this);
	State_Selete();
}

void CInvenItem::Late_Update(_float fTimeDelta)
{
	m_pItemSlot->Late_Update(fTimeDelta);
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::LATE_UI, this)))
		return;

}

HRESULT CInvenItem::Render()
{

	if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", m_iTexIndex)))
		return E_FAIL;

	__super::Bind_ShaderTransform_Resourc(m_iPassIndex);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CInvenItem::Initialize_Pool(void* pArg)
{
	ITEM_DESC* pDesc = static_cast<ITEM_DESC*>(pArg);
	m_ItemData = pDesc->ItemData;
	pDesc->vMinUV = {  pDesc->vMinUV.x / 2048 , pDesc->vMinUV.y / 2048 };
	pDesc->vMaxUV = { pDesc->vMaxUV.x / 2048 ,pDesc->vMaxUV.y / 2048};
	__super::Initialize_Pool(pDesc);

	CItem_Slot::ITEM_SLOT_DESC SlotDesc;
	SlotDesc.itemIndex = m_ItemData.iItemIndex;
	SlotDesc.vPos = m_vPos;

	if (FAILED(m_pItemSlot->Item_Init(SlotDesc)))
		return E_FAIL;

	CInven_Manager::GetInstance()->Add_Item(this);
	return S_OK;
}

void CInvenItem::Return_Pool()
{
}

void CInvenItem::Change_Slot(_float2 vPos, _int iGridX, _int iGridY)
{
	m_ItemData.iItemGridX = iGridX;
	m_ItemData.iItemGridY = iGridY;

	m_vLocalPos = vPos;
	m_vPos = m_vLocalPos;
	m_pItemSlot->Update_Pos(m_vPos);
}

HRESULT CInvenItem::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenItem::Ready_Children()
{
	m_pItemSlot = dynamic_cast<CItem_Slot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Item_Slot"), nullptr));

	if (m_pItemSlot == nullptr)
		return E_FAIL;

	return S_OK;
}

void CInvenItem::State_Selete()
{
	if (m_bIsSelete)
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		m_vPos.x = ptMouse.x;
		m_vPos.y = ptMouse.y;

		m_pItemSlot->Update_Pos(m_vPos);

		if (m_pGameInstance->IsMouseUp(MOUSEKEYSTATE::LB))
		{
			CInven_Manager::GetInstance()->Change_ItemSlot(this);
			m_bIsSelete = false;
		}
	}
	else
	{

		m_vPos = m_vLocalPos;
		m_pItemSlot->Update_Pos(m_vPos);
	}
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
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pItemSlot);
}
