#include "pch.h"
#include "Inventory_Tex.h"
#include "Engine_Defines.h"
CInventory_Tex::CInventory_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory_Tex::CInventory_Tex(const CInventory_Tex& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory_Tex::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CInventory_Tex::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    INVEN_TEX_DESC* Desc = static_cast<INVEN_TEX_DESC*>(pArg);

    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;
    m_iIndex = Desc->iIndex;
    m_fRotation = Desc->fRot;
    m_iTexIndex = Desc->iTexIndex;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    _float4 fRotation = { 0.f, 0.f, 1.f, 0.f };
    m_pTransformCom->Rotation(XMLoadFloat4(&fRotation), XMConvertToRadians(m_fRotation));


    return S_OK;
}

void CInventory_Tex::Priority_Update(_float fTimeDelta)
{

}

void CInventory_Tex::Update(_float fTimeDelta)
{

}

void CInventory_Tex::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CInventory_Tex::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", m_iTexIndex)))
        return E_FAIL;

    Bind_ShaderTransform_Resourc(2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CInventory_Tex::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CInventory_Tex* CInventory_Tex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory_Tex* pInstance = new CInventory_Tex(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Tex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory_Tex::Clone(void* pArg)
{
    CInventory_Tex* pInstance = new CInventory_Tex(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Tex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory_Tex::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
