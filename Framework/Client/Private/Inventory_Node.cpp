#include "pch.h"
#include "Inventory_Node.h"

CInventory_Node::CInventory_Node(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory_Node::CInventory_Node(const CInventory_Node& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory_Node::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CInventory_Node::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos.x = Desc->vPos.x + Desc->iIndex * Desc->OffsetX - (Desc->iMaxIndex - 1) * Desc->OffsetX / 2;;
    m_vLocalPos.y = Desc->vPos.y;
    m_vLocalSize = Desc->vSize;
    m_iIndex = Desc->iIndex;
    m_iMaxIndex = Desc->iMaxIndex;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    _float4 fRotation = { 0.f, 0.f, 1.f, 0.f };
    m_pTransformCom->Rotation(XMLoadFloat4(&fRotation), XMConvertToRadians(m_fRotation));


    return S_OK;
}

void CInventory_Node::Priority_Update(_float fTimeDelta)
{

}

void CInventory_Node::Update(_float fTimeDelta)
{

}

void CInventory_Node::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CInventory_Node::Render()
{

    Bind_ShaderTransform_Resourc(2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CInventory_Node::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CInventory_Node* CInventory_Node::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory_Node* pInstance = new CInventory_Node(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Node"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory_Node::Clone(void* pArg)
{
    CInventory_Node* pInstance = new CInventory_Node(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Node"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory_Node::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
