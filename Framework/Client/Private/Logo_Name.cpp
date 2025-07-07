#include "pch.h"
#include "Logo_Name.h"

CLogo_Name::CLogo_Name(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLogo_Name::CLogo_Name(const CLogo_Name& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLogo_Name::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogo_Name::Initialize(void* pArg)
{
    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos.x = 0.f;
    m_vLocalPos.y = Desc->OffsetY;
    m_vLocalSize.x = 512.f;
    m_vLocalSize.y = 512.f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    

    return S_OK;
}

void CLogo_Name::Priority_Update(_float fTimeDelta)
{
}

void CLogo_Name::Update(_float fTimeDelta)
{
 

}

void CLogo_Name::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLogo_Name::Render()
{
    __super::Bind_ShaderTex_Resourec(m_pShaderCom,0, m_pTextureCom);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CLogo_Name::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo_Name"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CLogo_Name* CLogo_Name::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_Name* pInstance = new CLogo_Name(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Name"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLogo_Name::Clone(void* pArg)
{
    CLogo_Name* pInstance = new CLogo_Name(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Name"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLogo_Name::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
