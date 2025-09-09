#include "pch.h"
#include "Quick_Slot_Icon.h"
#include "QuicK_Slot_Item.h"

CQuick_Slot_Icon::CQuick_Slot_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CQuick_Slot_Icon::CQuick_Slot_Icon(const CQuick_Slot_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CQuick_Slot_Icon::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CQuick_Slot_Icon::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_iIndex = Desc->iIndex;
    switch (Desc->iIndex)
    {
    case 1:
        m_vLocalPos.x = -Desc->OffsetX;
        m_vLocalPos.y = 0.f;
        break;
    case 2:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = -Desc->OffsetX;
        break;
    case 3:
        m_vLocalPos.x = Desc->OffsetX;
        m_vLocalPos.y = 0.f;
        break;
    case 4:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = Desc->OffsetX;
        break;
    }
    m_vLocalSize = Desc->vSize;
    m_vBackGroundColor = { 0.2f, 0.2f, 0.2f, 0.9f };
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    if (FAILED(Ready_ItemSlot()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Equip_QuickSlot>([&](const Event_Equip_QuickSlot& e)
        { if (m_iIndex == e.iQuickSlotIndex) Set_Item(e.iItemIndex); });
      
    return S_OK;
}

void CQuick_Slot_Icon::Priority_Update(_float fTimeDelta)
{
    m_pItemSlot->Priority_Update(fTimeDelta);
}

void CQuick_Slot_Icon::Update(_float fTimeDelta)
{
    switch (m_iITemType)
    {
    case 0:
        m_vSize.x = m_vLocalSize.x;
        
        break;
    case 1:
        m_vSize.x = m_vLocalSize.x * 2.f;
        break;

    }

    m_pItemSlot->Update(fTimeDelta);
}

void CQuick_Slot_Icon::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    if(m_iItemIndex >= 0)
        m_pItemSlot->Late_Update(fTimeDelta);
}

HRESULT CQuick_Slot_Icon::Render()
{
        if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 1)))
        return E_FAIL;

    m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
    
    if (m_iITemType == 1)
    {
        if (m_iIndex == 1)
        {
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f - (m_vLocalSize.x * 0.5f), -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
        }
        else if (m_iIndex == 3)
        {
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f + (m_vLocalSize.x * 0.5f), -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
        }
        else
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
    }
    else
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
    


    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    m_pShaderCom->Begin(8);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CQuick_Slot_Icon::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CQuick_Slot_Icon::Ready_ItemSlot()
{
    
    UIOBJECT_DESC Desc;
    Desc.iIndex = m_iIndex;
    Desc.vPos = m_vPos;
    Desc.vSize.x = m_vSize.x * 0.95f;
    Desc.vSize.y = m_vSize.y * 0.95f;

    Desc.vMinUV = { 0.f, 0.f};
    Desc.vMaxUV = { 1.f, 1.f };
    m_pItemSlot = dynamic_cast<CQuick_Slot_Item*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot_Item"), &Desc));
    if (m_pItemSlot == nullptr)
        return E_FAIL;

    Add_Child(this, m_pItemSlot, m_pShaderCom, m_pTextureCom);
    Safe_AddRef(m_pItemSlot);

    return S_OK;
}

void CQuick_Slot_Icon::Set_Item(_int iItemIndex)
{
    m_iItemIndex = iItemIndex;
    m_iITemType = g_ItemData[m_iItemIndex].m_iQuickSlotType;
    m_pItemSlot->Set_Item(iItemIndex);

    _wstring szText = TEXT("'") + g_ItemData[m_iItemIndex].m_szName + TEXT("' À» Äü½½·Ô¿¡ ÀåÂøÇÏ¿´½À´Ï´Ù.");
    m_pGameInstance->Publish(Event_Announce_UI_OPEN{ 1, (_uint)iItemIndex,szText, RENDERGROUP::LATE_UI});
}

CQuick_Slot_Icon* CQuick_Slot_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CQuick_Slot_Icon* pInstance = new CQuick_Slot_Icon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuick_Slot_Icon::Clone(void* pArg)
{
    CQuick_Slot_Icon* pInstance = new CQuick_Slot_Icon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuick_Slot_Icon::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pItemSlot);
}
