#include "pch.h"
#include "Aim_Pistol.h"
#include "Aim_Pistol_Line.h"
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

    return S_OK;
}

void CAim_Pistol::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CAim_Pistol::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CAim_Pistol::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
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
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Pistol_Line"),
        CAim_Pistol_Line::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CAim_Pistol::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    UIOBJECT_DESC Desc;
    _float fTexSize = 512.f;

    
    Desc.iMaxIndex = 4;
    Desc.vSize = { 9.f, 22.f };
    Desc.vMinUV = { 121.f / fTexSize, 23.f / fTexSize };
    Desc.vMaxUV = { 130.f / fTexSize, 45.f / fTexSize };
    Desc.OffsetX = 30.f;
    
    for (_uint i = 1; i <= Desc.iMaxIndex; i++)
    {
        Desc.iIndex = i;
      
        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Pistol_Line"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom);
    }


    return S_OK;
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
    Safe_Release(m_pTextureCom);
}
