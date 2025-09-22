#include "pch.h"
#include "Guide_Paper.h"
#include "UI_Tex.h"
#include "Guide_Paper_Tex.h"

CGuide_Paper::CGuide_Paper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CGuide_Paper::CGuide_Paper(const CGuide_Paper& Prototype) : CUIObject(Prototype)
{
}

HRESULT CGuide_Paper::Initialize_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Paper"),
        CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Guide/Paper.png"), 1))))
        return E_FAIL;

    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CGuide_Paper::Initialize(void* pArg)
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

    m_pGameInstance->Subscribe<EVENT_GUIDE_PAPER>([&](const EVENT_GUIDE_PAPER& e)
        { m_bIsOpen ? m_bIsOpen = false : m_bIsOpen = true;       });

    return S_OK;
}

void CGuide_Paper::Priority_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    __super::Priority_Update(fTimeDelta);
}

void CGuide_Paper::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    __super::Update(fTimeDelta);
}

void CGuide_Paper::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT CGuide_Paper::Render()
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

HRESULT CGuide_Paper::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Paper"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGuide_Paper::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Guide_Paper_Tex"),
        CGuide_Paper_Tex::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CGuide_Paper::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Guide_Paper_Tex")));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    return S_OK;
}

CGuide_Paper* CGuide_Paper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGuide_Paper* pInstance = new CGuide_Paper(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CGuide_Paper"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGuide_Paper::Clone(void* pArg)
{
    CGuide_Paper* pInstance = new CGuide_Paper(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CGuide_Paper"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGuide_Paper::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
