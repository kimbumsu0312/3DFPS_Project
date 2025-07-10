#include "pch.h"
#include "Aim_Line.h"

CAim_Line::CAim_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAim_Line::CAim_Line(const CAim_Line& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAim_Line::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAim_Line::Initialize(void* pArg)
{
    AIM_LINE_DESC* Desc = static_cast<AIM_LINE_DESC*>(pArg);
    m_iIndex = Desc->iIndex;
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
    m_pGameInstance->Subscribe<Event_HUD_GUN_AIM>([&](const Event_HUD_GUN_AIM& e) { 
        if (!e.bIsAIm) { m_bZoom = false; m_vPos = m_vMovePos; m_pGameInstance->Publish(Event_HUD_GUN{ false }); }
        else { m_bZoom = true; m_vMovePos = m_vPos; ;}});

    return S_OK;
}

void CAim_Line::Priority_Update(_float fTimeDelta)
{
}

void CAim_Line::Update(_float fTimeDelta)
{
    Aim_Zoom();
}

void CAim_Line::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CAim_Line::Render()
{
    Bind_ShaderTransform_Resourc();

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CAim_Line::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

void CAim_Line::Aim_Zoom()
{
    if (m_bZoom)
    {
        _float2 iNum = {};

        switch (m_iIndex)
        {
        case 1:
            if (g_iWinSizeY * 0.5f > m_vPos.y)
                m_vPos.y += 10.f;
            else
                m_pGameInstance->Publish(Event_HUD_GUN{ true });
            break;
        case 2:
            if (g_iWinSizeX * 0.5f < m_vPos.x)
                m_vPos.x -= 10.f;
            else
                m_pGameInstance->Publish(Event_HUD_GUN{ true });
            break;
        case 3:
            if (g_iWinSizeY * 0.5f < m_vPos.y)
                m_vPos.y -= 10.f;
            else
                m_pGameInstance->Publish(Event_HUD_GUN{ true });
            break;
        case 4:
            if (g_iWinSizeX * 0.5f > m_vPos.x)
                m_vPos.x += 10.f;
            else
                m_pGameInstance->Publish(Event_HUD_GUN{ true });
            break;
        }

        Correct_Position(m_vPos);

    }
}

CAim_Line* CAim_Line::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAim_Line* pInstance = new CAim_Line(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAim_Line::Clone(void* pArg)
{
    CAim_Line* pInstance = new CAim_Line(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAim_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAim_Line::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
