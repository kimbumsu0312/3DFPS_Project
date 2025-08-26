#include "pch.h"
#include "Get_UI.h"

CGet_UI::CGet_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CGet_UI::CGet_UI(const CGet_UI& Prototype) : CUIObject(Prototype)
{
}

HRESULT CGet_UI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGet_UI::Initialize(void* pArg)
{
    m_vLocalPos.x = 0.f;
    m_vLocalPos.y = 30.f;
    m_vLocalSize.x = 220.f;
    m_vLocalSize.y = 50.f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_vMinUV = { 2.f/512.f, 130.f/256.f };
    m_vMaxUV = { 77.f / 512.f, 203.f / 256.f };
    return S_OK;
}

void CGet_UI::Priority_Update(_float fTimeDelta)
{
    m_bIsOn = false;
}

void CGet_UI::Update(_float fTimeDelta)
{

}

void CGet_UI::Late_Update(_float fTimeDelta)
{

}

HRESULT CGet_UI::Render()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    _float4 vVector = { 0.f, g_iWinSizeX, 0.f, g_iWinSizeY };

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &vVector, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;
    _float fAlpha = 1.f;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(9);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    Render_Font();
    return S_OK;
}

HRESULT CGet_UI::Ready_Components()
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

void CGet_UI::Render_Font()
{
    _wstring szText = TEXT("'F'키 아이템 줍기");
    _float2 vFontPos = {};
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y;

    m_pGameInstance->DrawText(TEXT("Font_Godic"), szText.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });
    if(m_bIsOn)
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szText.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });
    else
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szText.c_str(), vFontPos, _fvector{ 0.4f, 0.4f, 0.4f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 0.6f, 0.6f });

}

void CGet_UI::Update_WorldPos(_vector vPos)
{
    _matrix OldVeiw = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
    _matrix OldProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

    _vector vWinPos{};
    _matrix vVPMatrix = OldVeiw * OldProj;

    vWinPos = XMVector3TransformCoord(vPos, vVPMatrix);
    
    _float fWinPosX = (XMVectorGetX(vWinPos) + 1.f) * 0.5 * g_iWinSizeX;
    _float fWinPosY = (1.f - XMVectorGetY(vWinPos)) * 0.5 * g_iWinSizeY;

    m_vPos.x = fWinPosX + m_vLocalPos.x;
    m_vPos.y = fWinPosY + m_vLocalPos.y;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::PRIORITY_UI, this)))
        return;
}

CGet_UI* CGet_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGet_UI* pInstance = new CGet_UI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CGet_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGet_UI::Clone(void* pArg)
{
    CGet_UI* pInstance = new CGet_UI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CGet_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGet_UI::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);

}
