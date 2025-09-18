#include "pch.h"
#include "Loding_Icon.h"

CLoding_Icon::CLoding_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLoding_Icon::CLoding_Icon(const CLoding_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLoding_Icon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLoding_Icon::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX - (56.f * 0.5f) - 22.f;
    m_vLocalPos.y = g_iWinSizeY - (56.f * 0.5f) - 22.f;
    //m_vLocalSize.x = 44;
    //m_vLocalSize.y = 44;

    m_vLocalSize.x = 44;
    m_vLocalSize.y = 44;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_vMinUV = {0.f, 0.f};
    m_vMaxUV = {44.f / 128.f, 44.f / 128.f};

    m_pTransformCom->Set_RotationPerSec(1.f);
    return S_OK;
}

void CLoding_Icon::Priority_Update(_float fTimeDelta)
{
}

void CLoding_Icon::Update(_float fTimeDelta)
{
    m_pTransformCom->Turn(_vector{ 0.f, 0.f, 1.f, 0.f }, XMConvertToRadians(fTimeDelta * -90.f));
}

void CLoding_Icon::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLoding_Icon::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(11);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CLoding_Icon::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loding_Icon"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CLoding_Icon* CLoding_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLoding_Icon* pInstance = new CLoding_Icon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLoding_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLoding_Icon::Clone(void* pArg)
{
    CLoding_Icon* pInstance = new CLoding_Icon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CLoding_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLoding_Icon::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
