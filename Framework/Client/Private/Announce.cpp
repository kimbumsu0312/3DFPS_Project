#include "pch.h"
#include "Announce.h"
#include "UI_Tex.h"
#include "UI_Slot.h"
CAnnounce::CAnnounce(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAnnounce::CAnnounce(const CAnnounce& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAnnounce::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CAnnounce::Initialize(void* pArg)
{
    m_fAlpha = 1.f;

    m_vLocalPos.x = g_iWinSizeX * 0.5f;
    m_vLocalPos.y = g_iWinSizeY * 0.7f;
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

    return S_OK;
}

void CAnnounce::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_M))
        UIOpen();
}

void CAnnounce::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    UIAlpha_OnOff(fTimeDelta);

    __super::Update(fTimeDelta);
}

void CAnnounce::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
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

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
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

    TexDesc.vPos = { 0.f, 55.f };
    TexDesc.vSize = { 450.f, 50.f };
    TexDesc.vMinUV = { 0.f / fTexSizeX , 214.f / fTexSizeY };
    TexDesc.vMaxUV = { 194.f / fTexSizeX , 250.f / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"), &TexDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    return S_OK;
}

void CAnnounce::UIOpen()
{
    if (!m_bIsOpen == true)
    {
        m_bIsOpen = true;
        m_bIsClose = false;
        m_fAlpha = 0.f;
    }
    else
    {
        m_fIsOpenTime = 2.f;
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
}
