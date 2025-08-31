#include "pch.h"
#include "InvenItem.h"
#include "Item_Slot.h"
#include "Inven_Manager.h"
#include "Player_Manager.h"

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
	if (!m_bIsRender)
		return;

	m_pItemSlot->Priority_Update(fTimeDelta);
}

void CInvenItem::Update(_float fTimeDelta)
{
	if (!m_bIsRender)
		return;

	if (m_bIsClick)
	{
		if (m_pGameInstance->IsKeyDown(DIK_1))
		{
			m_pGameInstance->Publish(Event_Equip_QuickSlot{ (_uint)m_ItemData.iItemIndex, 1 });
			CPlayer_Manager::GetInstance()->Add_QuickSlotItem(1, m_ItemData.iItemIndex);
		}
		if (m_pGameInstance->IsKeyDown(DIK_2))
		{
			m_pGameInstance->Publish(Event_Equip_QuickSlot{ (_uint)m_ItemData.iItemIndex, 2 });
			CPlayer_Manager::GetInstance()->Add_QuickSlotItem(2, m_ItemData.iItemIndex);
		}
		if (m_pGameInstance->IsKeyDown(DIK_3))
		{
			m_pGameInstance->Publish(Event_Equip_QuickSlot{ (_uint)m_ItemData.iItemIndex, 3 });
			CPlayer_Manager::GetInstance()->Add_QuickSlotItem(3, m_ItemData.iItemIndex);
		}
		if (m_pGameInstance->IsKeyDown(DIK_4))
		{
			m_pGameInstance->Publish(Event_Equip_QuickSlot{ (_uint)m_ItemData.iItemIndex, 4 });
			CPlayer_Manager::GetInstance()->Add_QuickSlotItem(4, m_ItemData.iItemIndex);
		}
	}

	m_pItemSlot->Update(fTimeDelta, this);
	State_Selete();
}

void CInvenItem::Late_Update(_float fTimeDelta)
{
	if (!m_bIsRender)
		return;

	m_pItemSlot->Late_Update(fTimeDelta, m_bIsSelete);

	if (m_bIsSelete)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI_EFFECT, this)))
			return;
	}
	else
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::LATE_UI, this)))
			return;
	}
}

HRESULT CInvenItem::Render()
{
	
	if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", m_iTexIndex)))
		return E_FAIL;

	_float fAlpha = 1.f;
	if (m_bIsClick)
	{
		fAlpha = 0.5f;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
			return E_FAIL;
		__super::Bind_ShaderTransform_Resourc(m_iPassIndex);
	}
	else
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
			return E_FAIL;
		__super::Bind_ShaderTransform_Resourc(2);
	}
	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	if (g_ItemData[m_ItemData.iItemIndex].m_iMaxItem > 1)
	{
		Render_Font();
	}
	return S_OK;
}

HRESULT CInvenItem::Initialize_Pool(void* pArg)
{
	ITEM_DESC* pDesc = static_cast<ITEM_DESC*>(pArg);
	m_ItemData = pDesc->ItemData;
	m_bIsRender = pDesc->IsRender;
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

void CInvenItem::IsSelete(_bool IsSelete)
{
	m_bIsPreRotation = m_bIsCulRotation;
	m_bIsSelete = IsSelete;
}

void CInvenItem::IsClick(_bool IsClick)
{
	m_bIsClick = IsClick;
}

void CInvenItem::IsRotation(_bool IsRotation)
{
	if (IsRotation)
	{
		m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 1.f), XMConvertToRadians(90.f));
		m_pItemSlot->IsRotation(90.f);
	}
	else
	{
		m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 1.f), XMConvertToRadians(0.f));
		m_pItemSlot->IsRotation(0.f);
	}
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

		m_vPos.x = (_float)ptMouse.x;
		m_vPos.y = (_float)ptMouse.y;
	
		m_pItemSlot->Update_Pos(m_vPos);

		if (m_pGameInstance->IsKeyDown(DIK_R))
		{
			m_bIsCulRotation ? m_bIsCulRotation = false : m_bIsCulRotation = true;
			IsRotation(m_bIsCulRotation);
		}

		if (m_pGameInstance->IsMouseUp(MOUSEKEYSTATE::LB))
		{
			if (false == CInven_Manager::GetInstance()->Change_ItemSlot(this))
			{
				m_bIsCulRotation = m_bIsPreRotation;
				IsRotation(m_bIsCulRotation);
			}
			else
			{
				if (m_bIsCulRotation != m_bIsPreRotation)
				{
					_int Size = m_ItemData.iInvenSizeX;
					m_ItemData.iInvenSizeX = m_ItemData.iInvenSizeY;
					m_ItemData.iInvenSizeY = Size;
				}
			}
			m_bIsSelete = false;
		}
	
	}
	else
	{

		m_vPos = m_vLocalPos;
		m_pItemSlot->Update_Pos(m_vPos);
	}
}

void CInvenItem::Render_Font()
{
	_float fSlotSize = (_float)CInven_Manager::GetInstance()->Get_InvenData().iSlotSize;

	if (!m_bIsCulRotation)
	{
		m_ItemRect.left = _int(m_vPos.x - (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeX * 0.5f));
		m_ItemRect.top = _int(m_vPos.y - (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeY * 0.5f));
		m_ItemRect.right = _int(m_vPos.x + (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeX * 0.5f) - 10);
		m_ItemRect.bottom = _int(m_vPos.y + (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeY * 0.5f) - 5);
	}
	else
	{
		m_ItemRect.left = _int(m_vPos.x - (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeY * 0.5f));
		m_ItemRect.top = _int(m_vPos.y - (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeX * 0.5f));
		m_ItemRect.right = _int(m_vPos.x + (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeY * 0.5f) - 10);
		m_ItemRect.bottom = _int(m_vPos.y + (fSlotSize * g_ItemData[m_ItemData.iItemIndex].m_iInvenSizeX * 0.5f) - 5);
	}
	_tchar szCountChar[MAX_PATH] = {  };
	wsprintf(szCountChar, L"%d", m_ItemData.iItemCount);
	
	if (m_ItemData.iItemCount <= 0)
	{
		m_pGameInstance->DrawText(TEXT("Font_Godic"), szCountChar, _float2((_float)m_ItemRect.right + 2, (_float)m_ItemRect.bottom + 2), _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 1.f, 1.f }, { 0.7f, 0.7f });
		m_pGameInstance->DrawText(TEXT("Font_Godic"), szCountChar, _float2((_float)m_ItemRect.right, (_float)m_ItemRect.bottom), _fvector{ 0.8f, 0.4f, 0.f, 1.f }, 0.f, _float2{ 1.f, 1.f }, { 0.7f, 0.7f });
	}
	else if(m_ItemData.iItemCount == g_ItemData[m_ItemData.iItemIndex].m_iMaxItem)
	{
		m_pGameInstance->DrawText(TEXT("Font_Godic"), szCountChar, _float2((_float)m_ItemRect.right + 2, (_float)m_ItemRect.bottom + 2), _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 1.f, 1.f }, { 0.7f, 0.7f });
		m_pGameInstance->DrawText(TEXT("Font_Godic"), szCountChar, _float2((_float)m_ItemRect.right, (_float)m_ItemRect.bottom), _fvector{ 0.f, 0.7f, 0.7f, 1.f }, 0.f, _float2{ 1.f, 1.f }, { 0.7f, 0.7f });
	}
	else
	{
		m_pGameInstance->DrawText(TEXT("Font_Godic"), szCountChar, _float2((_float)m_ItemRect.right + 2, (_float)m_ItemRect.bottom + 2), _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 1.f, 1.f }, { 0.7f, 0.7f });
		m_pGameInstance->DrawText(TEXT("Font_Godic"), szCountChar, _float2((_float)m_ItemRect.right, (_float)m_ItemRect.bottom), _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 1.f, 1.f }, { 0.7f, 0.7f });
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
