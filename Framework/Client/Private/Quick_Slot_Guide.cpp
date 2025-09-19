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
    _tchar szCountChar[MAX_PATH] = {  };
    wsprintf(szCountChar, L"%d", m_iIndex);

    m_szIndex = szCountChar;

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
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;
    m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));

    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    m_pShaderCom->Begin(8);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();
    Render_Font();
    return S_OK;
}

HRESULT CQuick_Slot_Guide::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

void CQuick_Slot_Guide::Render_Font()
{
    _float2 vFontPos = {};
    vFontPos.x = m_vPos.x - 1;
    vFontPos.y = m_vPos.y - 1;
 
    m_pGameInstance->DrawText(TEXT("Font_Godic"), m_szIndex.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });
    m_pGameInstance->DrawText(TEXT("Font_Godic"), m_szIndex.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });

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
