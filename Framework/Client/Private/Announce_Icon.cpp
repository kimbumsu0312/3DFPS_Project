#include "pch.h"
#include "Announce_Icon.h"

CAnnounce_Icon::CAnnounce_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CAnnounce_Icon::CAnnounce_Icon(const CAnnounce_Icon& Prototype) : CUIObject(Prototype)
{
}

HRESULT CAnnounce_Icon::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CAnnounce_Icon::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
    m_vLocalSize = Desc->vSize;
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CAnnounce_Icon::Priority_Update(_float fTimeDelta)
{
    if (m_iItemIndex < 0)
        return;
}

void CAnnounce_Icon::Update(_float fTimeDelta)
{
    if (m_iItemIndex < 0)
        return;
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

void CAnnounce_Icon::Late_Update(_float fTimeDelta)
{
    if (m_iItemIndex < 0)
        return;

    if(m_bIsRender)
       if (FAILED(m_pGameInstance->Add_RenderGroup(m_eRenderGroup, this)))
           return;

}

HRESULT CAnnounce_Icon::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 4)))
        return E_FAIL;

    if (m_iITemType == 1)
    {
        m_pTransformCom->Scale(_float3(m_vSize.x * 0.9f, m_vSize.y * 0.7f, 1.f));
    }
    else
    {
        m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
    }

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));


    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    m_pShaderCom->Begin(8);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CAnnounce_Icon::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

void CAnnounce_Icon::Set_Item(_int iItemIndex)
{
    if (iItemIndex == -1)
        m_bIsRender = false;
    else
    {
        m_bIsRender = true;
        m_iItemIndex = iItemIndex;
        m_iITemType = g_ItemData[m_iItemIndex].m_iQuickSlotType;
        m_vMinUV = { g_ItemData[m_iItemIndex].m_vMinUV.x / 2048.f, g_ItemData[m_iItemIndex].m_vMinUV.y / 2048.f };
        m_vMaxUV = { g_ItemData[m_iItemIndex].m_vMaxUV.x / 2048.f,g_ItemData[m_iItemIndex].m_vMaxUV.y / 2048.f };
    }
}

void CAnnounce_Icon::Set_RenderGroup(RENDERGROUP eRenderGroup)
{
    m_eRenderGroup = eRenderGroup;
}

CAnnounce_Icon* CAnnounce_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    
    CAnnounce_Icon* pInstance = new CAnnounce_Icon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CAnnounce_Icon"));
        Safe_Release(pInstance);
    }
    
    return pInstance;
}

CGameObject* CAnnounce_Icon::Clone(void* pArg)
{
    CAnnounce_Icon* pInstance = new CAnnounce_Icon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAnnounce_Icon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnnounce_Icon::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
