#include "pch.h"
#include "Screen_Blood.h"

CScreen_Blood::CScreen_Blood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CScreen_Blood::CScreen_Blood(const CScreen_Blood& Prototype) : CUIObject(Prototype)
{
}

HRESULT CScreen_Blood::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen_Blood::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;
    m_fAlpha = 0.f;
    m_vColor = { 0.924f, 0.235f, 0.157f, 1.f };
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

   m_pGameInstance->Subscribe<Event_OnDamageUI_OPEN>([&](const Event_OnDamageUI_OPEN& e) { m_fAlpha = 0.5f; });

    return S_OK;
}

void CScreen_Blood::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_J))
        m_fAlpha = 0.5f;
}

void CScreen_Blood::Update(_float fTimeDelta)
{
    if (m_fAlpha < 0.f)
        return;

    m_fAlpha -= fTimeDelta * (0.1f + m_fAlpha * 1.5f);
}

void CScreen_Blood::Late_Update(_float fTimeDelta)
{
    if (m_fAlpha < 0.f)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::PRIORITY_UI, this)))
        return;
}

HRESULT CScreen_Blood::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(13);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CScreen_Blood::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CScreen_Blood* CScreen_Blood::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen_Blood* pInstance = new CScreen_Blood(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CScreen_Blood"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CScreen_Blood::Clone(void* pArg)
{
    CScreen_Blood* pInstance = new CScreen_Blood(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CScreen_Blood"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CScreen_Blood::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
