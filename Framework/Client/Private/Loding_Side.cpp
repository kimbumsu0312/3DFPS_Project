#include "pch.h"
#include "Loding_Side.h"

CLoding_SideTex::CLoding_SideTex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLoding_SideTex::CLoding_SideTex(const CLoding_SideTex& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLoding_SideTex::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CLoding_SideTex::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vBackGroundColor = { 0.f, 0.f, 0.f, 0.9f };

    m_vLocalPos.x = g_iWinSizeX >> 1;
    if(pDesc->iIndex == 0)
        m_vLocalPos.y = g_iWinSizeY - (56.f * 0.5f);
    else
        m_vLocalPos.y = (56.f * 0.5f);

    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = 144.f;
    
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_vBackGroundColor = { 0.f, 0.f, 0.f, 1.f };

    return S_OK;
}

void CLoding_SideTex::Priority_Update(_float fTimeDelta)
{
}

void CLoding_SideTex::Update(_float fTimeDelta)
{
}

void CLoding_SideTex::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLoding_SideTex::Render()
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

HRESULT CLoding_SideTex::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CLoding_SideTex* CLoding_SideTex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLoding_SideTex* pInstance = new CLoding_SideTex(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLoding_SideTex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLoding_SideTex::Clone(void* pArg)
{
    CLoding_SideTex* pInstance = new CLoding_SideTex(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLoding_SideTex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLoding_SideTex::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
