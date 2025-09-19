#include "pch.h"
#include "Item_Info.h"
#include "Engine_Defines.h"
CItem_Info::CItem_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CItem_Info::CItem_Info(const CItem_Info& Prototype) : CUIObject(Prototype)
{
}

HRESULT CItem_Info::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CItem_Info::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UI_TEX_DESC* Desc = static_cast<UI_TEX_DESC*>(pArg);

    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;
    m_iIndex = Desc->iIndex;
    m_fRotation = Desc->fRot;
    m_iTexIndex = Desc->iTexIndex;
    m_iPassIndex = Desc->iPassIndex;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    _float4 fRotation = { 0.f, 0.f, 1.f, 0.f };
    m_pTransformCom->Rotation(XMLoadFloat4(&fRotation), XMConvertToRadians(m_fRotation));

    m_pGameInstance->Subscribe<Event_Inven_Info>([&](const Event_Inven_Info& e)
        { m_iItemIndex = e.iItemIndex; });

    m_pGameInstance->Subscribe<Event_Inventory_Open>([&](const Event_Inventory_Open& e) { m_fAlpha = 0.f; });

    return S_OK;
}

void CItem_Info::Priority_Update(_float fTimeDelta)
{
    m_iItemIndex = { -1 };
    m_fAlpha += fTimeDelta * 0.5f;
}

void CItem_Info::Update(_float fTimeDelta)
{

}

void CItem_Info::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CItem_Info::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource_IndexCheck(m_pShaderCom, "g_Texture", m_iTexIndex)))
        return E_FAIL;

    Bind_ShaderTransform_Resourc(m_iPassIndex);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();
    if(m_iItemIndex >= 0)
       Render_Font();

    return S_OK;
}

HRESULT CItem_Info::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

void CItem_Info::Render_Font()
{
    _float2 vFontPos = {};
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y - 30;

    _wstring szName = g_ItemData[m_iItemIndex].m_szName;
    m_pGameInstance->DrawText(TEXT("Font_Godic"), szName.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.8f, 0.8f });
    m_pGameInstance->DrawText(TEXT("Font_Godic"), szName.c_str(), vFontPos, _fvector{1.f, 1.f, 1.f, m_fAlpha }, 0.f, _float2{0.5f, 0.5f}, {0.8f, 0.8f});

    szName = g_ItemData[m_iItemIndex].m_szInfo;
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y + 5;

    m_pGameInstance->DrawText(TEXT("Font_Godic"), szName.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });
    m_pGameInstance->DrawText(TEXT("Font_Godic"), szName.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });
    
    if (g_ItemData[m_iItemIndex].m_eQuickSlot == QUICKSLOT_TYPE::EQUIP)
    {
        szName = TEXT("Äü½½·Ô µî·Ï °¡´É");
        vFontPos.x = m_vPos.x;
        vFontPos.y = m_vPos.y + 30;

        m_pGameInstance->DrawText(TEXT("Font_Godic"), szName.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.55f, 0.55f });
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szName.c_str(), vFontPos, _fvector{ 0.7f, 0.7f, 0.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.55f, 0.55f });
    }
}

CItem_Info* CItem_Info::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem_Info* pInstance = new CItem_Info(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CItem_Info"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_Info::Clone(void* pArg)
{
    CItem_Info* pInstance = new CItem_Info(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CItem_Info"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_Info::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
