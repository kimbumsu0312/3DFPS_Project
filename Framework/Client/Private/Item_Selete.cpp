#include "pch.h"
#include "Item_Selete.h"
#include "Inven_Manager.h"
#include "Item_Slot.h"
#include "Item_QuickSlot.h"

CItem_Selete::CItem_Selete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CButton{ pDevice, pContext }
{
}

CItem_Selete::CItem_Selete(const CItem_Selete& Prototype) : CButton(Prototype)
{
}

HRESULT CItem_Selete::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Selete::Initialize(void* pArg)
{
	m_vLocalSize = { 0.f, 0.f };
	m_vLocalPos = { 0.f, 0.f };
	m_fColor = { 1.0f, 1.0f, 0.2f, 0.3f };

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Childeren()))
		return E_FAIL;

	m_pGameInstance->Subscribe<Event_Equip_QuickSlot>([&](const Event_Equip_QuickSlot& e)
		{ m_bIsClick = false; });


	return S_OK;
}

void CItem_Selete::Priority_Update(_float fTimeDelta)
{

}

void CItem_Selete::Update(_float fTimeDelta, CInvenItem* pItem)
{
	
	if (m_fAlphaTime <= 1.f)
	{
		m_fAlphaTime += 2.f * fTimeDelta;
		m_fAlpha -= 2.f * fTimeDelta;
		
		if (m_fAlphaTime > 1.f)
			m_bIsSelete = true;
	}

	if (m_bIsClick)
		m_fAlpha = 1.f;
}

void CItem_Selete::Late_Update(_float fTimeDelta, _bool bIsSelete)
{
	if (m_bIsSelete || m_bIsClick)
	{
		if (bIsSelete)
		{
			if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI_EFFECT, this)))
				return;
		}
		else
		{
			if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::LATE_UI, this)))
				return;

			if(m_bIsClick)
				for (_int i = 0; i < 5; ++i)
					m_pItemGuide[i]->Late_Update(fTimeDelta, m_vPos);
		}
		
		

	}
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Selete::Render()
{

	if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;

	if (m_bIsClick)
	{
		m_pShaderCom->Bind_RawValue("g_vColor", &m_fColor, sizeof(_float4));
		m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));
		__super::Bind_ShaderTransform_Resourc(6);
	}
	else
	{
		m_pShaderCom->Bind_RawValue("g_Alpha",&m_fAlpha, sizeof(_float));
		__super::Bind_ShaderTransform_Resourc(4);
	}
	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CItem_Selete::Item_Init(const ITEM_SELETE_DESC& pDesc)
{
	_float fSlotSize = CInven_Manager::GetInstance()->Get_InvenData().iSlotSize;

	m_iItemIndex = pDesc.itemIndex;
	m_vPos = pDesc.vPos;

	m_vSize.x = fSlotSize * g_ItemData[m_iItemIndex].m_iInvenSizeX;
	m_vSize.y = fSlotSize * g_ItemData[m_iItemIndex].m_iInvenSizeY;
	Default_Slot();

	return S_OK;
}

void CItem_Selete::Update_Pos(_float2 vPos)
{
	m_vPos = vPos;
}

void CItem_Selete::IsRotation(_float fAngle)
{
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 1.f), XMConvertToRadians(fAngle));
}

void CItem_Selete::IsSelete(_bool IsSelete)
{
	if (IsSelete)
	{
		m_bIsSelete = true;
		m_fAlpha = 1.f;
		m_fAlphaTime = 0.f;
	}
}

void CItem_Selete::IsClick(_bool IsSelete)
{
	m_bIsClick = IsSelete;
}

_bool CItem_Selete::IsClick()
{
	m_bIsClick ? m_bIsClick = false : m_bIsClick = true;
	return m_bIsClick;
}

HRESULT CItem_Selete::Ready_Components()
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

HRESULT CItem_Selete::Ready_Childeren()
{
	CItem_QuikSlot* pGameObject = nullptr;

	UIOBJECT_DESC Desc;
	Desc.iMaxIndex = 4;
	Desc.vSize = { 60.f, 60.f };
	Desc.vMinUV = { 79.f / 512.f, 133.f / 256.f };
	Desc.vMaxUV = { 155.f / 512.f, 206.f / 256.f };
	Desc.OffsetX = 50.f;

	Desc.iIndex = 0;

	pGameObject = static_cast<CItem_QuikSlot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Item_Slot_Quick"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_pItemGuide[0] = pGameObject;
	Desc.vSize = { 30.f, 30.f };

	Desc.vMinUV = { 216.f / 512.f, 166.f / 256.f };
	Desc.vMaxUV = { 245.f / 512.f, 195.f / 256.f };
	for (_uint i = 1; i <= Desc.iMaxIndex; i++)
	{
		Desc.iIndex = i;

		pGameObject = static_cast<CItem_QuikSlot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Item_Slot_Quick"), &Desc));
		if (nullptr == pGameObject)
			return E_FAIL;
		m_pItemGuide[i] = pGameObject;
	}


	return S_OK;
}

void CItem_Selete::Default_Slot()
{
	m_vMinUV = { 62.f / 512.f , 62.f / 512.f };
	m_vMaxUV = { 117.f / 512.f , 117.f / 512.f };
	m_iPassIndex = 2;
}

CItem_Selete* CItem_Selete::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Selete* pInstance = new CItem_Selete(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Selete"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Selete::Clone(void* pArg)
{
	CItem_Selete* pInstance = new CItem_Selete(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Selete"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Selete::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);

	
	for (_int i = 0; i < 5; ++i)
	{
		Safe_Release(m_pItemGuide[i]);
	}
}
