#include "pch.h"
#include "Guide_Paper_Tex.h"

CGuide_Paper_Tex::CGuide_Paper_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CGuide_Paper_Tex::CGuide_Paper_Tex(const CGuide_Paper_Tex& Prototype) : CUIObject(Prototype)
{
}

HRESULT CGuide_Paper_Tex::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGuide_Paper_Tex::Initialize(void* pArg)
{
    m_vLocalPos = { 0.f, 0.f };
    m_vLocalSize.x = 1280.f * 0.5f;
    m_vLocalSize.y = 1080.f * 0.8;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pGameInstance->Subscribe<EVENT_GUIDE_PAPER>([&](const EVENT_GUIDE_PAPER& e)
        { m_iTexType = e.iTexType; });


    return S_OK;
}

void CGuide_Paper_Tex::Priority_Update(_float fTimeDelta)
{
}

void CGuide_Paper_Tex::Update(_float fTimeDelta)
{
}

void CGuide_Paper_Tex::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CGuide_Paper_Tex::Render()
{

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(3);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    switch (m_iTexType)
    {
    case ENUM_CLASS(TEXT_TYPE::EVENT1):
        Render_Font_EVENT_1();
        break;
    case ENUM_CLASS(TEXT_TYPE::EVENT2):
        Render_Font_EVENT_2();
        break;
    case ENUM_CLASS(TEXT_TYPE::EVENT3):
        Render_Font_EVENT_3();
        break;
    case ENUM_CLASS(TEXT_TYPE::EVENT4):
        Render_Font_EVENT_4();
        break;
    }

    return S_OK;
}

HRESULT CGuide_Paper_Tex::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

void CGuide_Paper_Tex::Render_Font_EVENT_1()
{
    _wstring szFont = {};

    _float2 vFontPos = {};
        
    szFont = TEXT("정원 왼쪽 건물 1층 방으로 오세요");
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y - 25.f;;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });

    szFont = TEXT("거기서 기다리고 있겠습니다.");
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y + 25.f;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });

}

void CGuide_Paper_Tex::Render_Font_EVENT_2()
{
    _wstring szFont = {};

    _float2 vFontPos = {};

    szFont = TEXT("나가시려면");
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y - 75.f;;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });


    szFont = TEXT("딸들이랑 놀아주셔야 해요");
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y - 25.f;;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });


    szFont = TEXT("막내는 2층으로 가면 있습니다.");
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y + 25.f;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });

}

void CGuide_Paper_Tex::Render_Font_EVENT_3()
{
    _wstring szFont = TEXT("뒤를 봐");

    _float2 vFontPos = {};
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });

}

void CGuide_Paper_Tex::Render_Font_EVENT_4()
{
    _wstring szFont = TEXT("이벤트 4번 테스트");

    _float2 vFontPos = {};
    vFontPos.x = m_vPos.x;
    vFontPos.y = m_vPos.y;

    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), _float2{ vFontPos.x + 2, vFontPos.y + 2 }, _fvector{ 0.f, 0.f, 0.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });
    m_pGameInstance->DrawText(TEXT("Font_GangBuJang"), szFont.c_str(), vFontPos, _fvector{ 1.f, 1.f, 1.f, 1.f }, 0.f, _float2{ 0.5f, 0.5f }, { 1.f, 1.f });

}

CGuide_Paper_Tex* CGuide_Paper_Tex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGuide_Paper_Tex* pInstance = new CGuide_Paper_Tex(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CGuide_Paper_Tex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGuide_Paper_Tex::Clone(void* pArg)
{
    CGuide_Paper_Tex* pInstance = new CGuide_Paper_Tex(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CGuide_Paper_Tex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGuide_Paper_Tex::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
