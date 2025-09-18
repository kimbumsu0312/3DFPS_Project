#include "pch.h"
#include "LodingUI.h"

CLodingUI::CLodingUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLodingUI::CLodingUI(const CLodingUI& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLodingUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLodingUI::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_eFade = FadeTag::FADE_IN;

    m_pGameInstance->Subscribe<Event_EndLoding>([&](const Event_EndLoding& e) {m_bisNext = true; });
    return S_OK;
}

void CLodingUI::Priority_Update(_float fTimeDelta)
{
}

void CLodingUI::Update(_float fTimeDelta)
{
    if (m_eFade == FadeTag::FADE_IN)
    {
        m_fFadeOut += fTimeDelta * 0.5f;
        if (m_fFadeOut >= 1.f)
        {
            m_eFade = FadeTag::END;
        }
    }
    else if (m_eFade == FadeTag::FADE_OUT)
    {
        m_fFadeOut -= fTimeDelta * 0.5f;
        if (m_fFadeOut <= 0.f)
        {
            if(m_bisNext)
                m_pGameInstance->Publish(Event_NextLevel{});

            m_eFade = FadeTag::FADE_IN;
            ++m_iTexIndex;
            if (m_iTexIndex >= 10)
                m_iTexIndex = 0;
        }
    }
    else
    {
        m_fTime += fTimeDelta;
        if (m_fTime >= 2.f)
        {
            m_fTime = 0.f;
            m_eFade = FadeTag::FADE_OUT;
        }
    }
}

void CLodingUI::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLodingUI::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_FadeOut", &m_fFadeOut, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", m_iTexIndex)))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(10);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CLodingUI::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loding"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CLodingUI* CLodingUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLodingUI* pInstance = new CLodingUI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLodingUI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLodingUI::Clone(void* pArg)
{
    CLodingUI* pInstance = new CLodingUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CLodingUI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLodingUI::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
