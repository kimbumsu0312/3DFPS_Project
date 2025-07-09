#include "pch.h"
#include "Aim_Pistol_Line.h"

CAim_Pistol_Line::CAim_Pistol_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAim_Pistol_Line::CAim_Pistol_Line(const CAim_Pistol_Line& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAim_Pistol_Line::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAim_Pistol_Line::Initialize(void* pArg)
{
    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
    
    switch (Desc->iIndex)
    {
    case 1:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = -Desc->OffsetX;
        m_fRotation = 0.f;
        break;
    case 2:
        m_vLocalPos.x = Desc->OffsetX;
        m_vLocalPos.y = 0.f;
        m_fRotation = 90.f;
        break;
    case 3:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = Desc->OffsetX;
        m_fRotation = 0.f;
        break;
    case 4:
        m_vLocalPos.x = -Desc->OffsetX;
        m_vLocalPos.y = 0.f;
        m_fRotation = 90.f;
        break;
    }
    m_vLocalSize = Desc->vSize;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    _float4 fRotation = { 0.f, 0.f, 1.f, 0.f };

    m_pTransformCom->Rotation(XMLoadFloat4(&fRotation), XMConvertToRadians(m_fRotation));

    return S_OK;
}

void CAim_Pistol_Line::Priority_Update(_float fTimeDelta)
{
}

void CAim_Pistol_Line::Update(_float fTimeDelta)
{
}

void CAim_Pistol_Line::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CAim_Pistol_Line::Render()
{
    Bind_ShaderTransform_Resourc();

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CAim_Pistol_Line::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CAim_Pistol_Line* CAim_Pistol_Line::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAim_Pistol_Line* pInstance = new CAim_Pistol_Line(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Pistol_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAim_Pistol_Line::Clone(void* pArg)
{
    CAim_Pistol_Line* pInstance = new CAim_Pistol_Line(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Pistol_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAim_Pistol_Line::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
