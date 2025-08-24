#include "pch.h"
#include "Item_QuickSlot.h"

CItem_QuikSlot::CItem_QuikSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CItem_QuikSlot::CItem_QuikSlot(const CItem_QuikSlot& Prototype) : CUIObject(Prototype)
{
}

HRESULT CItem_QuikSlot::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CItem_QuikSlot::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_iIndex = Desc->iIndex;
    _tchar szCountChar[MAX_PATH] = {  };
    wsprintf(szCountChar, L"%d", m_iIndex);

    m_szIndex = szCountChar;

    switch (Desc->iIndex)
    {
    case 0:
        m_vLocalPos.x = 0.f;
        m_vLocalPos.y = 0.f;
        break;
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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CItem_QuikSlot::Priority_Update(_float fTimeDelta)
{
}

void CItem_QuikSlot::Update(_float fTimeDelta)
{
}

void CItem_QuikSlot::Late_Update(_float fTimeDelta, _float2 vPos)
{
    m_vPos.x = vPos.x + m_vLocalPos.x;
    m_vPos.y = vPos.y + m_vLocalPos.y;
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI_EFFECT, this)))
        return;

}

HRESULT CItem_QuikSlot::Render()
{

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;
    m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");
    
    _float fAlpha = 2.f;
    if (m_iIndex == 0)
       fAlpha = 0.7f;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    m_pShaderCom->Begin(8);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();
    if (m_iIndex != 0)
        Render_Font();
    return S_OK;
}

HRESULT CItem_QuikSlot::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

void CItem_QuikSlot::Render_Font()
{
    _float2 vFontPos = {};
    vFontPos.x = m_vPos.x - 1;
    vFontPos.y = m_vPos.y - 1;

    m_pGameInstance->DrawText(TEXT("Font_Godic"), m_szIndex.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 0.8f }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });
    m_pGameInstance->DrawText(TEXT("Font_Godic"), m_szIndex.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 0.8f }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });

}

CItem_QuikSlot* CItem_QuikSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem_QuikSlot* pInstance = new CItem_QuikSlot(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CItem_QuikSlot"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_QuikSlot::Clone(void* pArg)
{
    CItem_QuikSlot* pInstance = new CItem_QuikSlot(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CItem_QuikSlot"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_QuikSlot::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
