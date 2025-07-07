#include "pch.h"
#include "Inventory_Base.h"

CInventory_Base::CInventory_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory_Base::CInventory_Base(const CInventory_Base& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory_Base::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInventory_Base::Initialize(void* pArg)
{
    m_vLocalPos.x = 0.f;
    m_vLocalPos.y = 0.f;
    m_vLocalSize.x = 1280 * 0.7;
    m_vLocalSize.y = 1080 * 0.7;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    return S_OK;
}

void CInventory_Base::Priority_Update(_float fTimeDelta)
{
}

void CInventory_Base::Update(_float fTimeDelta)
{

}

void CInventory_Base::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CInventory_Base::Render()
{
    __super::Bind_ShaderTex_Resourec(m_pShaderCom,3, m_pTextureCom, 5);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CInventory_Base::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory_Base::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    return S_OK;
}

CInventory_Base* CInventory_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory_Base* pInstance = new CInventory_Base(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Base"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory_Base::Clone(void* pArg)
{
    CInventory_Base* pInstance = new CInventory_Base(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Base"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory_Base::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
