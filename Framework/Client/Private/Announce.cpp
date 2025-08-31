#include "pch.h"
#include "Announce.h"
#include "UI_Tex.h"
#include "UI_Slot.h"
#include "Announce_Icon.h"

CAnnounce::CAnnounce(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAnnounce::CAnnounce(const CAnnounce& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAnnounce::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAnnounce::Initialize(void* pArg)
{
    m_fAlpha = 1.f;

    m_vLocalPos.x = g_iWinSizeX * 0.5f;
    m_vLocalPos.y = g_iWinSizeY * 0.8f;
    m_vLocalSize.x = 0.f;
    m_vLocalSize.y = 0.f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    _float fMaxSizeX = 512;
    _float fMaxSizeY = 256;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Announce_UI_OPEN>([&](const Event_Announce_UI_OPEN& e)
        { UIOpen(e.iType, e.iItemIndex, e.szText, e.eRenderGroup); });

    return S_OK;
}

void CAnnounce::Priority_Update(_float fTimeDelta)
{
}

void CAnnounce::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    UIAlpha_OnOff(fTimeDelta);

    if(m_iITemType == -1)
        m_pSlot->Set_Size({ 0.f, 0.f });
    else if (m_iITemType == 1)
        m_pSlot->Set_Size({ 64.f * 2.f, 64.f });
    else
        m_pSlot->Set_Size({ 64.f, 64.f });
    m_pInfo->Update_Alpha(0.f);
    __super::Update(fTimeDelta);
}

void CAnnounce::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(m_eRnderGroup, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT CAnnounce::Render()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    _float4 vVector = { 0.f, g_iWinSizeX, 0.f, g_iWinSizeY };

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &vVector, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;

    return S_OK;
}

HRESULT CAnnounce::Ready_Components()
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

HRESULT CAnnounce::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CUI_Tex::UI_TEX_DESC  TexDesc;

    _float fTexSizeX = 512.f;
    _float fTexSizeY = 256.f;

    TexDesc.vPos = { 0.f, 0.f };
    TexDesc.vSize = { 64.f, 64.f };
    TexDesc.vMinUV = { 0.f , 0.f };
    TexDesc.vMaxUV = { 64.f / fTexSizeX , 64.f / fTexSizeY };
    TexDesc.iTexIndex = 3;
    TexDesc.iPassIndex = 4;
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"), &TexDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    m_pSlot = static_cast<CUI_Tex*>(pGameObject);
    Safe_AddRef(m_pSlot);

    m_pSlot->Set_RenderGroup(RENDERGROUP::PRIORITY_UI);
    TexDesc.vPos = { 0.f, 75.f };
    TexDesc.vSize = { 500.f, 70.f };
    TexDesc.vMinUV = { 0.f / fTexSizeX , 214.f / fTexSizeY };
    TexDesc.vMaxUV = { 194.f / fTexSizeX , 250.f / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"), &TexDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    m_pInfo = static_cast<CUI_Tex*>(pGameObject);
    Safe_AddRef(m_pInfo);
    m_pInfo->Set_RenderGroup(RENDERGROUP::PRIORITY_UI);

    UIOBJECT_DESC Desc;
    Desc.vPos = m_vPos;
    Desc.vSize.x = 64.f * 0.95f;
    Desc.vSize.y = 64.f * 0.95f;

    Desc.vMinUV = { 0.f, 0.f };
    Desc.vMaxUV = { 1.f, 1.f };
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Announce_Icon"), &Desc));
    if (pGameObject == nullptr)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    m_pIcon = static_cast<CAnnounce_Icon*>(pGameObject);
    Safe_AddRef(m_pIcon);

    return S_OK;
}

void CAnnounce::UIOpen(_int iType, _int iItemIndex, _wstring szFont, RENDERGROUP eRenderGroup)
{
    m_eRnderGroup = eRenderGroup;
    m_pSlot->Set_RenderGroup(m_eRnderGroup);
    m_pInfo->Set_RenderGroup(m_eRnderGroup);
    m_pIcon->Set_RenderGroup(m_eRnderGroup);
    if (iType == 0)
    {
        if (!m_bIsOpen == true)
        {
            m_iITemType = g_ItemData[iItemIndex].m_iQuickSlotType;
            m_bIsOpen = true;
            m_bIsClose = false;
            m_fAlpha = 0.f;
            m_pIcon->Set_Item(iItemIndex);

            _wstring szGetFont = TEXT("'") + g_ItemData[iItemIndex].m_szName + TEXT("' À» È¹µæÇÏ¿´½À´Ï´Ù.");
            m_pInfo->Set_Font(true, szGetFont);
            m_fIsOpenTime = 2.f;
        }
        else
        {
            m_iITemType = g_ItemData[iItemIndex].m_iQuickSlotType;
            m_fAlpha = 0.f;
            m_pIcon->Set_Item(iItemIndex);
            _wstring szGetFont = TEXT("'") + g_ItemData[iItemIndex].m_szName + TEXT("' À» È¹µæÇÏ¿´½À´Ï´Ù.");
            m_pInfo->Set_Font(true, szGetFont);

            m_fIsOpenTime = 2.f;
        }
    }
    else if(iType == 1)
    {
        if (!m_bIsOpen == true)
        {
            m_iITemType = -1;
            m_bIsOpen = true;
            m_bIsClose = false;
            m_fAlpha = 0.f;
            m_pIcon->Set_Item(-1);
            m_pInfo->Set_Font(true, szFont);
            m_fIsOpenTime = 2.f;

        }
        else
        {
            m_iITemType = -1;
            m_fAlpha = 0.f;
            m_pIcon->Set_Item(-1);
            m_pInfo->Set_Font(true, szFont);
            m_fIsOpenTime = 2.f;
        }
    }
}

void CAnnounce::UIAlpha_OnOff(_float fTimeDelta)
{
    if (m_fIsOpenTime > 0.f && !m_bIsClose)
    {
        m_fIsOpenTime -= fTimeDelta;
        if (m_fIsOpenTime <= 0.f)
            m_bIsClose = true;
    }

    if (!m_bIsClose && m_fAlpha < 1.f)
    {
        m_fAlpha += fTimeDelta;
        if (m_fAlpha >= 1.f)
            m_fIsOpenTime = 2.5f;
    }

    if (m_bIsClose && m_bIsOpen)
    {
        if (m_fAlpha >= 0.f)
            m_fAlpha -= fTimeDelta;
        else {
            m_bIsOpen = false;
            m_bIsClose = false;
        }
    }
}

HRESULT CAnnounce::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Announce_Icon"),
        CAnnounce_Icon::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    
    return S_OK;
}

CAnnounce* CAnnounce::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAnnounce* pInstance = new CAnnounce(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CAnnounce"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAnnounce::Clone(void* pArg)
{
    CAnnounce* pInstance = new CAnnounce(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAnnounce"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnnounce::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pIcon);
    Safe_Release(m_pInfo);
    Safe_Release(m_pSlot);
}
