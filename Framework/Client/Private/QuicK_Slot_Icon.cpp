#include "pch.h"
#include "Quick_Slot_Icon.h"

CQuick_Slot_Icon::CQuick_Slot_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CQuick_Slot_Icon::CQuick_Slot_Icon(const CQuick_Slot_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CQuick_Slot_Icon::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CQuick_Slot_Icon::Initialize(void* pArg)
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

void CQuick_Slot_Icon::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_O))
        m_iITemType = 0;

    if (m_pGameInstance->IsKeyDown(DIK_P))
        m_iITemType = 1;
}

void CQuick_Slot_Icon::Update(_float fTimeDelta)
{
    switch (m_iITemType)
    {
    case 0:
        m_vSize.x = m_vLocalSize.x;
        
        break;
    case 1:
        m_vSize.x = m_vLocalSize.x * 2.f;
        break;

    }
}

void CQuick_Slot_Icon::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CQuick_Slot_Icon::Render()
{

    //Bind_ShaderTransform_Resourc(4);
    m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
    
    if (m_iITemType == 1)
    {
        if (m_iIndex == 1)
        {
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - m_iWinSizeX * 0.5f - (m_vLocalSize.x * 0.5f), -m_vPos.y + m_iWinSizeY * 0.5f, 0.0f, 1.0f));
        }
        else if (m_iIndex == 3)
        {
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - m_iWinSizeX * 0.5f + (m_vLocalSize.x * 0.5f), -m_vPos.y + m_iWinSizeY * 0.5f, 0.0f, 1.0f));
        }
        else
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - m_iWinSizeX * 0.5f, -m_vPos.y + m_iWinSizeY * 0.5f, 0.0f, 1.0f));
    }
    else
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - m_iWinSizeX * 0.5f, -m_vPos.y + m_iWinSizeY * 0.5f, 0.0f, 1.0f));
    


    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    m_pShaderCom->Begin(4);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CQuick_Slot_Icon::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CQuick_Slot_Icon* CQuick_Slot_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CQuick_Slot_Icon* pInstance = new CQuick_Slot_Icon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CQuick_Slot_Icon::Clone(void* pArg)
{
    CQuick_Slot_Icon* pInstance = new CQuick_Slot_Icon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CQuick_Slot_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CQuick_Slot_Icon::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
