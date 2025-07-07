#include "pch.h"
#include "Fade_UI.h"

CFade_UI::CFade_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CFade_UI::CFade_UI(const CFade_UI& Prototype) : CUIObject(Prototype)
{
}

HRESULT CFade_UI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFade_UI::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;
    m_fFadeTime = 1.f;
    m_fAlpah = 1.f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;
    m_pGameInstance->Subscribe<Event_Level_Change>([&](const Event_Level_Change& e) { m_bIsFadeIn = true; });

    return S_OK;
}

void CFade_UI::Priority_Update(_float fTimeDelta)
{
}

void CFade_UI::Update(_float fTimeDelta)
{

    if (m_bIsFadeIn)
    {
        m_fAlpah += m_fFadeTime * fTimeDelta;
        m_vFadeValue.w = m_fAlpah;
        if (m_fAlpah > 1.f)
        {
            m_vFadeValue.w = 1.f;
            m_pGameInstance->Publish(Event_NextLevel{});
        }
    }
    else if (m_bIsFadeOut)
    {
        m_fAlpah -= m_fFadeTime * fTimeDelta;
        m_vFadeValue.w = m_fAlpah;
        if (m_fAlpah < 0.f)
        {
            m_bIsFadeOut = false;
            m_fAlpah = 0.f;
        }
    }

    
}

void CFade_UI::Late_Update(_float fTimeDelta)
{
    if (m_bIsFadeOut || m_bIsFadeIn)
        if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
            return;
}

HRESULT CFade_UI::Render()
{
    if (FAILED(m_pShaderCom->Bind_Vector("g_Vector", XMLoadFloat4(&m_vFadeValue))))
        return E_FAIL;

    __super::Bind_Shader_Resourec(m_pShaderCom, 2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CFade_UI::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CFade_UI::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    return S_OK;
}

CFade_UI* CFade_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFade_UI* pInstance = new CFade_UI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CFade_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFade_UI::Clone(void* pArg)
{
    CFade_UI* pInstance = new CFade_UI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CFade_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFade_UI::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
