#include "pch.h"
#include "Item_Slot.h"
#include "Inven_Manager.h"

CItem_Slot::CItem_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CButton{ pDevice, pContext }
{
}

CItem_Slot::CItem_Slot(const CItem_Slot& Prototype) : CButton(Prototype)
{
}

HRESULT CItem_Slot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Slot::Initialize(void* pArg)
{
	m_vLocalSize = { 0.f, 0.f };
	m_vLocalPos = { 0.f, 0.f };

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CItem_Slot::Priority_Update(_float fTimeDelta)
{

}

void CItem_Slot::Update(_float fTimeDelta, CInvenItem* pItem)
{
	if (IsPick())
	{
		if (IsClick_Down(MOUSEKEYSTATE::LB))
		{
			pItem->IsSelete(true);
		}

		//if (IsClick_Up(MOUSEKEYSTATE::LB))
		//{
		//	pItem->IsSelete(false);
		//}
	}
}

void CItem_Slot::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::LATE_UI, this)))
		return;
}

HRESULT CItem_Slot::Render()
{
	if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;

	__super::Bind_ShaderTransform_Resourc(m_iPassIndex);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CItem_Slot::Item_Init(const ITEM_SLOT_DESC& pDesc)
{
	_float fSlotSize = CInven_Manager::GetInstance()->Get_InvenData().iSlotSize;

	m_iItemIndex = pDesc.itemIndex;
	m_vPos = pDesc.vPos;

	m_vSize.x = fSlotSize * g_ItemData[m_iItemIndex].m_iInvenSizeX;
	m_vSize.y = fSlotSize * g_ItemData[m_iItemIndex].m_iInvenSizeY;
	Default_Slot();
	return S_OK;
}

void CItem_Slot::Update_Pos(_float2 vPos)
{
	m_vPos = vPos;
}

HRESULT CItem_Slot::Ready_Components()
{

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

void CItem_Slot::Default_Slot()
{
	m_vMinUV = { 62.f / 512.f , 2.f / 512.f };
	m_vMaxUV = { 118.f / 512.f , 58.f / 512.f };
	m_iPassIndex = 2;
}

CItem_Slot* CItem_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Slot* pInstance = new CItem_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Slot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Slot::Clone(void* pArg)
{
	CItem_Slot* pInstance = new CItem_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Slot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
}
