#include "pch.h"
#include "Inventory_Coin.h"
CInventory_Coin::CInventory_Coin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory_Coin::CInventory_Coin(const CInventory_Coin& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory_Coin::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CInventory_Coin::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_vMinUV = { 0.f, 0.f };
    m_vMaxUV = { 1.f, 1.f };

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CInventory_Coin::Priority_Update(_float fTimeDelta)
{

}

void CInventory_Coin::Update(_float fTimeDelta)
{

}

void CInventory_Coin::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CInventory_Coin::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 4)))
        return E_FAIL;

    Bind_ShaderTransform_Resourc(2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CInventory_Coin::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CInventory_Coin* CInventory_Coin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory_Coin* pInstance = new CInventory_Coin(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Coin"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory_Coin::Clone(void* pArg)
{
    CInventory_Coin* pInstance = new CInventory_Coin(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Coin"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory_Coin::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
