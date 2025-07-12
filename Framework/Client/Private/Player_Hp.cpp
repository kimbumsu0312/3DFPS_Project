#include "pch.h"
#include "Player_Hp.h"
#include "Player_Hp_Tex.h"
#include "Player_Hp_Flash.h"

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

    return S_OK;
}

void CPlayer_Hp::Priority_Update(_float fTimeDelta)
{
}

void CPlayer_Hp::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CPlayer_Hp::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CPlayer_Hp::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
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
    Desc.vSize = { 235.f, 45.f };
    Desc.vMinUV = { 7 / fTexSizeX, 215 / fTexSizeY };
    Desc.vMaxUV = { 192 / fTexSizeX , 247 / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp_Flash"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    return S_OK;
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
