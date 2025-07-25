#include "pch.h"
#include "Logo_Button_Line.h"

CLogo_Button_Line::CLogo_Button_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLogo_Button_Line::CLogo_Button_Line(const CLogo_Button_Line& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLogo_Button_Line::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLogo_Button_Line::Initialize(void* pArg)
{
    m_vLocalPos.x = 0;
    m_vLocalPos.y = 0;
    m_vLocalSize.x = 512.f;
    m_vLocalSize.y = 512.f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CLogo_Button_Line::Priority_Update(_float fTimeDelta)
{
}

void CLogo_Button_Line::Update(_float fTimeDelta)
{
}

void CLogo_Button_Line::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLogo_Button_Line::Render()
{
    __super::Bind_Shader_Resourec(m_pShaderCom, m_pTextureCom, 0);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CLogo_Button_Line::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo_Line"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CLogo_Button_Line* CLogo_Button_Line::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_Button_Line* pInstance = new CLogo_Button_Line(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Button_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLogo_Button_Line::Clone(void* pArg)
{
    CLogo_Button_Line* pInstance = new CLogo_Button_Line(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Button_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLogo_Button_Line::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
