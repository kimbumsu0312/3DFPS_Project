#include "pch.h"
#include "Aim_Pistol.h"
#include "Aim_Line.h"
CAim_Pistol::CAim_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAim_Pistol::CAim_Pistol(const CAim_Pistol& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAim_Pistol::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAim_Pistol::Initialize(void* pArg)
{
    _float fTexSize = 512.f;
    m_vMinUV = { 128.f / fTexSize, 3.f / fTexSize };
    m_vMaxUV = { 139.f / fTexSize, 16.f / fTexSize };
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = 11.f;
    m_vLocalSize.y = 11.f;

    m_eWeapon_Type = WEAPON_TYPE::PISTOL;

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

void CAim_Pistol::Priority_Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;
    __super::Priority_Update(fTimeDelta);
}

void CAim_Pistol::Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;

    if (m_fCool > 0.f)
        m_fCool -= fTimeDelta;

    KeyInput();

    __super::Update(fTimeDelta);
}

void CAim_Pistol::Late_Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
         return;
    if (!m_bZoom)
        __super::Late_Update(fTimeDelta);
}

HRESULT CAim_Pistol::Render()
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

HRESULT CAim_Pistol::Ready_Components()
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

HRESULT CAim_Pistol::Ready_Children_Prototype()
{

    return S_OK;
}

HRESULT CAim_Pistol::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    CAim_Line::AIM_LINE_DESC Desc;
    _float fTexSize = 512.f;

    Desc.eType = WEAPON_TYPE::PISTOL;
    Desc.iMaxIndex = 4;
    Desc.vSize = { 9.f, 22.f };
    Desc.vMinUV = { 121.f / fTexSize, 23.f / fTexSize };
    Desc.vMaxUV = { 130.f / fTexSize, 45.f / fTexSize };
    Desc.OffsetX = 60.f;
    
    for (_uint i = 1; i <= Desc.iMaxIndex; i++)
    {
        Desc.iIndex = i;
      
        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Line"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    }


    return S_OK;
}

void CAim_Pistol::Shoting()
{
    //m_pGameInstance->Publish(Event_HUD_GUN_AIM{false});
    //m_pGameInstance->Publish(Event_Camera_Zoom{ CAMERA_STATE::ZOOM_RESET, 0.f, 0.f });
    //m_bIsZoomIn = false;
    m_fCool = 0.5f;
}

void CAim_Pistol::KeyInput()
{
    if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::RB))
    {
        if (!m_bIsZoomIn)
        {
            m_bIsZoomIn = true;
            m_pGameInstance->Publish(Event_HUD_GUN_AIM{ true });
            m_pGameInstance->Publish(Event_Camera_Zoom{ CAMERA_STATE::ZOOM_IN, 50.f, 50.f });
        }
    }

    if (m_pGameInstance->IsMouseUp(MOUSEKEYSTATE::RB))
    {
        m_bIsZoomIn = false;
        m_pGameInstance->Publish(Event_HUD_GUN_AIM{ false });
        m_pGameInstance->Publish(Event_Camera_Zoom{ CAMERA_STATE::ZOOM_RESET, 0.f, 0.f });
    }
}

CAim_Pistol* CAim_Pistol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAim_Pistol* pInstance = new CAim_Pistol(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Pistol"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAim_Pistol::Clone(void* pArg)
{
    CAim_Pistol* pInstance = new CAim_Pistol(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Pistol"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAim_Pistol::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
