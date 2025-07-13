#include "pch.h"
#include "Player_Hp_Flash.h"

CPlayer_Hp_Flash::CPlayer_Hp_Flash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CPlayer_Hp_Flash::CPlayer_Hp_Flash(const CPlayer_Hp_Flash& Prototype) : CUIObject(Prototype)
{
}

HRESULT CPlayer_Hp_Flash::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CPlayer_Hp_Flash::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;
    m_vColor = { 0.f, 0.6f, 0.f, 0.7f };
    m_fAlpha = 0.f;
    m_fFlashSpeed = 0.5f;
    m_bAlphaAdd = true;
    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Player_Hp_Set>([&](const Event_Player_Hp_Set& e) {m_vColor = e.Color; });

    return S_OK;
}

void CPlayer_Hp_Flash::Priority_Update(_float fTimeDelta)
{

}

void CPlayer_Hp_Flash::Update(_float fTimeDelta)
{
    if (m_bAlphaAdd)
    {
        m_fAlpha += m_fFlashSpeed * fTimeDelta;
        if (m_fAlpha >= 0.5f)
        {
            m_fAlpha = 0.5f;
            m_bAlphaAdd = false;
        }
    }
    else
    {
        m_fAlpha -= m_fFlashSpeed * fTimeDelta;
        if (m_fAlpha <= 0.f)
        {
            m_fAlpha = 0.f;
            m_bAlphaAdd = true;
        }
    }
    m_vColor.w = m_fAlpha;
}

void CPlayer_Hp_Flash::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CPlayer_Hp_Flash::Render()
{
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));
    Bind_ShaderTransform_Resourc(6);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CPlayer_Hp_Flash::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CPlayer_Hp_Flash* CPlayer_Hp_Flash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer_Hp_Flash* pInstance = new CPlayer_Hp_Flash(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp_Flash"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer_Hp_Flash::Clone(void* pArg)
{
    CPlayer_Hp_Flash* pInstance = new CPlayer_Hp_Flash(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp_Flash"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer_Hp_Flash::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
