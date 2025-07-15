#include "pch.h"
#include "Create_Penal_Slot.h"

CCreate_Penal_Slot::CCreate_Penal_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CCreate_Penal_Slot::CCreate_Penal_Slot(const CCreate_Penal_Slot& Prototype) : CUIObject(Prototype)
{
}

HRESULT CCreate_Penal_Slot::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CCreate_Penal_Slot::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;
    m_iIndex = Desc->iIndex;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CCreate_Penal_Slot::Priority_Update(_float fTimeDelta)
{

}

void CCreate_Penal_Slot::Update(_float fTimeDelta)
{

}

void CCreate_Penal_Slot::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CCreate_Penal_Slot::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", 5)))
         return E_FAIL;
    Bind_ShaderTransform_Resourc(2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CCreate_Penal_Slot::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CCreate_Penal_Slot* CCreate_Penal_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCreate_Penal_Slot* pInstance = new CCreate_Penal_Slot(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CCreate_Penal_Slot"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCreate_Penal_Slot::Clone(void* pArg)
{
    CCreate_Penal_Slot* pInstance = new CCreate_Penal_Slot(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CCreate_Penal_Slot"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCreate_Penal_Slot::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
