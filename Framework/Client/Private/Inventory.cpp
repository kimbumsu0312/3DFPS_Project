#include "pch.h"
#include "Inventory.h"
#include "Inventory_Base.h"
#include "Inventory_Tex.h"
#include "Inventory_Slot.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory::CInventory(const CInventory& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
    m_vBackGroundColor = { 0.f, 0.f, 0.f, 0.9f };

    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Inventory_Open>([&](const Event_Inventory_Open& e) { m_bIsOpen = e.bIsOpen; });

    return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    __super::Priority_Update(fTimeDelta);
}

void CInventory::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    __super::Update(fTimeDelta);
}

void CInventory::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT CInventory::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;


    if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &m_vBackGroundColor, sizeof(_float4))))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(1);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CInventory::Ready_Components()
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

HRESULT CInventory::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventory_Base"),
        CInventory_Base::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"),
        CInventory_Tex::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"),
        CInventory_Slot::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventory_Base")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory* pInstance = new CInventory(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
    CInventory* pInstance = new CInventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
