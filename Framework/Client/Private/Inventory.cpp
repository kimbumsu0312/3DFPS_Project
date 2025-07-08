#include "pch.h"
#include "Inventory.h"
#include "Inventory_Base.h"

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
    m_vBackGroundColor = { 0.f, 0.f, 0.f, 0.8f };

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
    if (FAILED(m_pShaderCom->Bind_Vector("g_Vector", XMLoadFloat4(&m_vBackGroundColor))))
        return E_FAIL;

    __super::Bind_Shader_Resourec(m_pShaderCom, 2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CInventory::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventory_Base"),
        CInventory_Base::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventory_Base")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject);

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
    Safe_Release(m_pShaderCom);
}
