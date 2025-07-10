#include "pch.h"
#include "Quick_Slot.h"
#include "Quick_Slot_Guide.h"
#include "QuicK_Slot_Icon.h"
CQuick_Slot::CQuick_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CQuick_Slot::CQuick_Slot(const CQuick_Slot& Prototype) : CUIObject(Prototype)
{
}

HRESULT CQuick_Slot::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CQuick_Slot::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX * 0.22;
    m_vLocalPos.y = g_iWinSizeY * 0.63;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    return S_OK;
}

void CQuick_Slot::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_7))
    {
        m_bIsOpen = true;
        m_bIsClose = false;
        m_fAlpha.x = 0.f;
    }
    if (m_pGameInstance->IsKeyDown(DIK_6))
    {
        m_bIsClose = true;
        m_fAlpha.x = 1.f;

    }
    __super::Priority_Update(fTimeDelta);
}

void CQuick_Slot::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (!m_bIsClose && m_fAlpha.x < 1.f)
        m_fAlpha.x += 0.1f;

    if (m_bIsClose && m_bIsOpen)
    {
        if (m_fAlpha.x >= 0.f)
            m_fAlpha.x -= 0.1f;
        else {
            m_bIsOpen = false;
            m_bIsClose = false;
        }
    }

    __super::Update(fTimeDelta);
}

void CQuick_Slot::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT CQuick_Slot::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Vector("g_Alpha", XMLoadFloat2(&m_fAlpha))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;
    
    return S_OK;
}

HRESULT CQuick_Slot::Ready_Components()
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

HRESULT CQuick_Slot::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot_Guide"),
        CQuick_Slot_Guide::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot_Icon"),
        CQuick_Slot_Icon::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CQuick_Slot::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    UIOBJECT_DESC Desc;

    Desc.iMaxIndex = 4;
    Desc.vSize = { 20.f, 20.f };
    Desc.OffsetX = 40.f;

    for (_uint i = 1; i <= Desc.iMaxIndex; i++)
    {
        Desc.iIndex = i;

        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot_Guide"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    }

    _float fTexSize = 512.f;

    Desc.iMaxIndex = 4;
    Desc.vSize = { 70.f, 70.f };
    Desc.vMinUV = { 61.f / fTexSize, 1.f / fTexSize };
    Desc.vMaxUV = { 118.f / fTexSize, 58.f / fTexSize };
    Desc.OffsetX = 90.f;

    for (_uint i = 1; i <= Desc.iMaxIndex; i++)
    {
        Desc.iIndex = i;

        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot_Icon"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    }
    return S_OK;
}

CQuick_Slot* CQuick_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CQuick_Slot* pInstance = new CQuick_Slot(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuick_Slot::Clone(void* pArg)
{
    CQuick_Slot* pInstance = new CQuick_Slot(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuick_Slot::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
