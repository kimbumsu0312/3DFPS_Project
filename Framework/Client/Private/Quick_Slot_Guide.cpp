#include "pch.h"
#include "Quick_Slot_Guide.h"

CQuick_Slot_Guide::CQuick_Slot_Guide(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CQuick_Slot_Guide::CQuick_Slot_Guide(const CQuick_Slot_Guide& Prototype) : CUIObject(Prototype)
{
}

HRESULT CQuick_Slot_Guide::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CQuick_Slot_Guide::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_iIndex = Desc->iIndex;
    switch (Desc->iIndex)
    {
    case 1:
        m_vLocalPos.x = -Desc->OffsetX;
        m_vLocalPos.y = 0.f;
        break;
    case 2:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = -Desc->OffsetX;
        break;
    case 3:
        m_vLocalPos.x = Desc->OffsetX;
        m_vLocalPos.y = 0.f;
        break;
    case 4:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = Desc->OffsetX;
        break;
    }
    m_vLocalSize = Desc->vSize;
    m_vBackGroundColor = { 0.2f, 0.2f, 0.2f, 0.9f };
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CQuick_Slot_Guide::Priority_Update(_float fTimeDelta)
{
}

void CQuick_Slot_Guide::Update(_float fTimeDelta)
{
}

void CQuick_Slot_Guide::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CQuick_Slot_Guide::Render()
{
    if (FAILED(m_pShaderCom->Bind_Vector("g_Vector", XMLoadFloat4(&m_vBackGroundColor))))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(5);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CQuick_Slot_Guide::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CQuick_Slot_Guide* CQuick_Slot_Guide::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CQuick_Slot_Guide* pInstance = new CQuick_Slot_Guide(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot_Guide"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuick_Slot_Guide::Clone(void* pArg)
{
    CQuick_Slot_Guide* pInstance = new CQuick_Slot_Guide(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot_Guide"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuick_Slot_Guide::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
