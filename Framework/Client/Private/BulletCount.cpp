#include "pch.h"
#include "BulletCount.h"

CBulletCount::CBulletCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CBulletCount::CBulletCount(const CBulletCount& Prototype) : CUIObject(Prototype)
{
}

HRESULT CBulletCount::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBulletCount::Initialize(void* pArg)
{
    m_iCulInvenBullet = 0;
    m_iCulGunBullet = 0;

    m_vLocalSize.x = 90.f * 3.f;
    m_vLocalSize.y = 21.f * 3.f;
    m_vLocalPos = { g_iWinSizeX - 50.f - (m_vLocalSize.x * 0.5f),g_iWinSizeY - 50.f - (m_vLocalSize.y * 0.5f) };

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_vMinUV = {145.f / 512.f, 89.f / 256.f};
    m_vMaxUV = {235.f / 512.f, 110.f / 256.f};

    m_pGameInstance->Subscribe<Event_BulletCount_UI_OPEN>([&](const Event_BulletCount_UI_OPEN& e)
        { 
            Open_UI(); 
            m_iItemIndex = e.iItemIndex;
            m_iCulGunBullet = e.iCulGunBullet;
            m_iCulInvenBullet = e.iCulInvenBullet;
        });
    return S_OK;
}

void CBulletCount::Priority_Update(_float fTimeDelta)
{

}

void CBulletCount::Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;

    if (!m_bIsClose && m_fAlpha < 1.f)
    {
        m_fAlpha += 0.1f;
    }
    else
    {
        m_fDelay += fTimeDelta;
        if (m_fDelay > 2.f)
            m_bIsClose = true;
    }
    if (m_bIsClose && m_bIsOpen)
    {
        if (m_fAlpha >= 0.f)
            m_fAlpha -= 0.1f;
        else {
            m_bIsOpen = false;
            m_bIsClose = false;
        }
    }
}

void CBulletCount::Late_Update(_float fTimeDelta)
{
    if (!m_bIsOpen)
        return;
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CBulletCount::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;

    m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));
    __super::Bind_ShaderTransform_Resourc(12);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();
    Render_Font();
    return S_OK;
}

void CBulletCount::Open_UI()
{
    if (!m_bIsOpen)
    {
        m_bIsOpen = true;
        m_bIsClose = false;
        m_fDelay = 0.f;
        m_fAlpha = 0.f;
    }
    else
    {
        m_bIsClose = false;
        m_fDelay = 0.f;
    }
}

HRESULT CBulletCount::Ready_Components()
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

void CBulletCount::Render_Font()
{
    
    _tchar szGunBulletChar[MAX_PATH] = {  };
    _tchar szInvenBulletChar[MAX_PATH] = {  };
    wsprintf(szGunBulletChar, L"%d", m_iCulGunBullet);
    wsprintf(szInvenBulletChar, L"%d", m_iCulInvenBullet);

    if (m_iCulGunBullet <= 0)
    {
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szGunBulletChar, _float2(m_vPos.x - 18, m_vPos.y +2), _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 1.f, 0.5f }, { 1.2f, 1.2f });
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szGunBulletChar, _float2(m_vPos.x - 20, m_vPos.y), _fvector{ 0.8f, 0.2f, 0.2f, m_fAlpha }, 0.f, _float2{ 1.f, 0.5f }, { 1.2f, 1.2f });
    }
    else if (m_iCulGunBullet == g_ItemData[m_iItemIndex].m_iMaxItem)
    {
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szGunBulletChar, _float2(m_vPos.x - 18, m_vPos.y + 2), _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 1.f, 0.5f }, { 1.2f, 1.2f });
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szGunBulletChar, _float2(m_vPos.x - 20, m_vPos.y), _fvector{ 0.f, 0.7f, 0.7f, m_fAlpha }, 0.f, _float2{ 1.f, 0.5f }, { 1.2f, 1.2f });
    }
    else
    {
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szGunBulletChar, _float2(m_vPos.x - 18, m_vPos.y + 2), _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 1.f, 0.5f }, { 1.2f, 1.2f });
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szGunBulletChar, _float2(m_vPos.x - 20, m_vPos.y), _fvector{ 1.f, 1.f, 1.f, m_fAlpha }, 0.f, _float2{ 1.f, 0.5f }, { 1.2f, 1.2f });
    }

    m_pGameInstance->DrawText(TEXT("Font_Godic"), TEXT("/"), _float2(m_vPos.x + 2, m_vPos.y + 2), _fvector{0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{0.5f, 0.5f}, { 1.2f, 1.2f });
    m_pGameInstance->DrawText(TEXT("Font_Godic"), TEXT("/"), m_vPos, _fvector{ 1.f, 1.f, 1.f, m_fAlpha }, 0.f, _float2{ 0.5f, 0.5f }, { 1.2f, 1.2f });

    if (m_iCulInvenBullet <= 0)
    {
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szInvenBulletChar, _float2(m_vPos.x + 22, m_vPos.y + 2), _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 0.f, 0.5f }, { 1.2f, 1.2f });
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szInvenBulletChar, _float2(m_vPos.x + 20, m_vPos.y), _fvector{ 0.8f, 0.2f, 0.2f, m_fAlpha }, 0.f, _float2{ 0.f, 0.5f }, { 1.2f, 1.2f });
    }
    else
    {
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szInvenBulletChar, _float2(m_vPos.x + 22, m_vPos.y + 2), _fvector{ 0.f, 0.f, 0.f, m_fAlpha }, 0.f, _float2{ 0.f, 0.5f }, { 1.2f, 1.2f });
        m_pGameInstance->DrawText(TEXT("Font_Godic"), szInvenBulletChar, _float2(m_vPos.x + 20, m_vPos.y), _fvector{ 1.f, 1.f, 1.f, m_fAlpha }, 0.f, _float2{ 0.f, 0.5f }, { 1.2f, 1.2f });
    }
}

CBulletCount* CBulletCount::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBulletCount* pInstance = new CBulletCount(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBulletCount"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBulletCount::Clone(void* pArg)
{
    CBulletCount* pInstance = new CBulletCount(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBulletCount"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBulletCount::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
