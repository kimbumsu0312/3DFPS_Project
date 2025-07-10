#include "pch.h"
#include "Aim_Shotgun.h"
#include "Aim_Shotgun_Line.h"

CAim_Shotgun::CAim_Shotgun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAim_Shotgun::CAim_Shotgun(const CAim_Shotgun& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAim_Shotgun::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAim_Shotgun::Initialize(void* pArg)
{
    _float fTexSize = 512.f;
    m_vMinUV = { 142.f / fTexSize, 94.f / fTexSize };
    m_vMaxUV = { 194.f / fTexSize, 144.f / fTexSize };
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = 50.f;
    m_vLocalSize.y = 50.f;

    m_eWeapon_Type = WEAPON_TYPE::SHOTGUN;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Weapon_Selete>([&](const Event_Weapon_Selete& e)
        { if (e.eType == m_eWeapon_Type) { m_bIsSelete = true; }
        else { m_bIsSelete = false; } });

    m_pGameInstance->Subscribe<Event_HUD_GUN>([&](const Event_HUD_GUN& e) { if (m_bIsSelete) { m_bZoom = e.bZoom; } });
    m_pGameInstance->Subscribe<Hud_Weapon_Shoting>([&](const Hud_Weapon_Shoting& e) { if (m_bIsSelete) { Shoting(); } });

    return S_OK;
}

void CAim_Shotgun::Priority_Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;
    __super::Priority_Update(fTimeDelta);
}

void CAim_Shotgun::Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;
    
    if (m_fCool > 0.f)
        m_fCool -= fTimeDelta;
    else
        KeyInput();

    __super::Update(fTimeDelta);
}

void CAim_Shotgun::Late_Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    if (!m_bZoom)
        __super::Late_Update(fTimeDelta);
}

HRESULT CAim_Shotgun::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (m_bZoom)
    {
        Bind_ShaderTransform_Resourc();

        m_pVIBufferCom->Bind_Resources();
        m_pVIBufferCom->Render();
    }
    return S_OK;
}

HRESULT CAim_Shotgun::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Hud_Aim"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CAim_Shotgun::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Shotgun_Line"),
        CAim_Shotgun_Line::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CAim_Shotgun::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    UIOBJECT_DESC Desc;
    _float fTexSize = 512.f;


    Desc.iMaxIndex = 2;
    Desc.vSize = { 23.f, 77.f };
    Desc.vMinUV = { 120.f / fTexSize, 95.f / fTexSize };
    Desc.vMaxUV = { 143.f / fTexSize, 188.f / fTexSize };
    Desc.OffsetX = 70.f;

    for (_uint i = 1; i <= Desc.iMaxIndex; i++)
    {
        Desc.iIndex = i;

        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Shotgun_Line"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    }


    return S_OK;
}

void CAim_Shotgun::KeyInput()
{
    if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::RB))
    {
        if (!m_bIsZoomIn)
        {
            m_bIsZoomIn = true;
            m_pGameInstance->Publish(Event_HUD_GUN_AIM{ true });
            m_pGameInstance->Publish(Event_Camera_Zoom{ CAMERA_STATE::ZOOM_IN, 30.f, 55.f });
        }
    }

    if (m_pGameInstance->IsMouseUp(MOUSEKEYSTATE::RB))
    {
        m_pGameInstance->Publish(Event_HUD_GUN_AIM{ false });
        m_pGameInstance->Publish(Event_Camera_Zoom{ CAMERA_STATE::ZOOM_RESET, 0.f, 0.f });
    }
}

void CAim_Shotgun::Shoting()
{
    m_pGameInstance->Publish(Event_HUD_GUN_AIM{ false });
    m_pGameInstance->Publish(Event_Camera_Zoom{ CAMERA_STATE::ZOOM_RESET, 0.f, 0.f });
    m_bIsZoomIn = false;
    m_fCool = 0.3f;
}

CAim_Shotgun* CAim_Shotgun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAim_Shotgun* pInstance = new CAim_Shotgun(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Shotgun"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAim_Shotgun::Clone(void* pArg)
{
    CAim_Shotgun* pInstance = new CAim_Shotgun(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Shotgun"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAim_Shotgun::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
