#include "pch.h"
#include "Player_Hp.h"
#include "Player_Hp_Tex.h"
#include "Player_Hp_Flash.h"
#include "Player_Hp_Beat.h"
#include "Player_Manager.h"
CPlayer_Hp::CPlayer_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CPlayer_Hp::CPlayer_Hp(const CPlayer_Hp& Prototype) : CUIObject(Prototype)
{
}

HRESULT CPlayer_Hp::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer_Hp::Initialize(void* pArg)
{
    m_fAlpha = 1.f;

    m_vLocalPos.x = g_iWinSizeX * 0.5f;
    m_vLocalPos.y = g_iWinSizeY * 0.5f;
    m_vLocalSize.x = 250.f;
    m_vLocalSize.y = 75.f;
    m_bIsInven = false;
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    _float fMaxSizeX = 512;
    _float fMaxSizeY = 256;

    m_vMinUV = {161 / fMaxSizeX, 185 / fMaxSizeY };
    m_vMaxUV = {198 / fMaxSizeX, 202 / fMaxSizeY };
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Player_Hp_UI_Open>([&](const Event_Player_Hp_UI_Open& e) { UIOpen_Damage(); });
    m_pGameInstance->Subscribe<Event_Inventory_Open>([&](const Event_Inventory_Open& e) { UIOpen_Inventory(e.bIsOpen); });

    return S_OK;
}

void CPlayer_Hp::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_4))
    {
        m_pGameInstance->Publish(Event_Player_Hp_UI_Open{ 0 });
    }
    if (m_pGameInstance->IsKeyDown(DIK_U))
    {
        CPlayer_Manager::GetInstance()->Player_Hp(-10);
    }
}

void CPlayer_Hp::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if(!m_bIsInven)
        UIAlpha_OnOff(fTimeDelta);

    __super::Update(fTimeDelta);
}

void CPlayer_Hp::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CPlayer_Hp::Render()
{
        if (!m_bIsInven)
    {
        _float4 vVector = { 0.f, g_iWinSizeX, 0.f, g_iWinSizeY };

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &vVector, sizeof(_float4))))
            return E_FAIL;
    }
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
            return E_FAIL;
    
    _float fSizeClip = 15.f;
    _float4 fSize = { m_vPos.x - m_vSize.x * 0.5f + fSizeClip , m_vPos.y - m_vSize.y * 0.5f , m_vPos.x + m_vSize.x * 0.5f - fSizeClip , m_vPos.y + m_vSize.y * 0.5f };

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Size", &fSize, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(4);
        
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CPlayer_Hp::Ready_Components()
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

HRESULT CPlayer_Hp::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Tex"),
        CPlayer_Hp_Tex::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Flash"),
        CPlayer_Hp_Flash::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Beat"),
        CPlayer_Hp_Beat::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer_Hp::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CUIObject::UIOBJECT_DESC Desc;
    _float fTexSizeX = 512.f;
    _float fTexSizeY = 256.f;

    Desc.vPos = { 0.f, 0.f };
    Desc.vSize = { 235.f, 65.f };
    Desc.vMinUV = { 237 / fTexSizeX, 93 / fTexSizeY };
    Desc.vMaxUV = { 440 / fTexSizeX , 124 / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    
    Desc.vPos = { -115.f, 0.f };
    Desc.vSize = { 20.f, 35.f };
    Desc.vMinUV = { 446 / fTexSizeX, 93 / fTexSizeY };
    Desc.vMaxUV = { 456 / fTexSizeX , 100 / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { 115.f, 0.f };
    Desc.vSize = { 20.f, 35.f };
    Desc.vMinUV = { 446 / fTexSizeX, 93 / fTexSizeY };
    Desc.vMaxUV = { 456 / fTexSizeX , 100 / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { 0.f, 0.f };
    Desc.vSize = { 245.f, 45.f };
    Desc.vMinUV = { 7 / fTexSizeX, 215 / fTexSizeY };
    Desc.vMaxUV = { 192 / fTexSizeX , 247 / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Flash"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { 0.f, 0.f };
    Desc.vSize = { 120.f, 50.f };
    Desc.OffsetX = { m_vSize.x };
    Desc.vMinUV = { 144 / fTexSizeX, 88 / fTexSizeY };
    Desc.vMaxUV = { 192 / fTexSizeX , 111 / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Beat"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    return S_OK;
}

void CPlayer_Hp::UIOpen_Damage()
{
    if (!m_bIsOpen == true)
    {
        m_bIsOpen = true;
        m_bIsClose = false;
        m_fAlpha = 0.f;
        m_vPos = { 150.f, g_iWinSizeY - 58 };
        Update_Position_Children(this);
    }
    else
    {
        m_fIsOpenTime = 2.f;
    }
}

void CPlayer_Hp::UIOpen_Inventory(_bool bIsOpen)
{
    if (bIsOpen)
    {
        m_bIsOpen = true;
        m_bIsInven = true;
        m_bIsClose = false;
        m_vPos = { 380.f, 140.f };
        Update_Position_Children(this);
        m_fAlpha = 1.f;
    }
    else
    {
        m_bIsOpen = false;
        m_bIsInven = false;
        m_bIsClose = false;

    }
}

void CPlayer_Hp::UIAlpha_OnOff(_float fTimeDelta)
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

CPlayer_Hp* CPlayer_Hp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer_Hp* pInstance = new CPlayer_Hp(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer_Hp::Clone(void* pArg)
{
    CPlayer_Hp* pInstance = new CPlayer_Hp(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer_Hp::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
