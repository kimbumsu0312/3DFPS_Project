#include "pch.h"
#include "Screen_Blood_Particle.h"

CScreen_Blood_Particle::CScreen_Blood_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CScreen_Blood_Particle::CScreen_Blood_Particle(const CScreen_Blood_Particle& Prototype) : CUIObject(Prototype)
{
}

HRESULT CScreen_Blood_Particle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen_Blood_Particle::Initialize(void* pArg)
{
    m_iTexSizeX = 4;
    m_iTexSizeY = 4;
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = 10.f;
    m_vLocalSize.y = 10.f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_OnDamageUI_OPEN>([&](const Event_OnDamageUI_OPEN& e) 
        {  ++m_iIndex;
    if (m_iIndex > 4)
        m_iIndex = 0;
    m_fAlpha = 0.5f;
    m_pVIBufferCom[m_iIndex]->Reset();
        });

    return S_OK;
}

void CScreen_Blood_Particle::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_J))
    {
        ++m_iIndex;
        if (m_iIndex > 4)
            m_iIndex = 0;
        m_fAlpha = 0.5f;
        m_pVIBufferCom[m_iIndex]->Reset();
    }
}

void CScreen_Blood_Particle::Update(_float fTimeDelta)
{
    if (m_fAlpha < 0.f)
        return;

    m_fAlpha -= fTimeDelta * 0.5f;
    m_pVIBufferCom[m_iIndex]->Spread(fTimeDelta);
}

void CScreen_Blood_Particle::Late_Update(_float fTimeDelta)
{
    if (m_fAlpha < 0.f)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::PRIORITY_UI, this)))
        return;
}

HRESULT CScreen_Blood_Particle::Render()
{
    m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - g_iWinSizeX * 0.5f, -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");
 
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_DiffuseTexture", 0)))
        return E_FAIL;

    m_pShaderCom->Bind_RawValue("g_iTexValueX", &m_iTexSizeX, sizeof(_int));
    m_pShaderCom->Bind_RawValue("g_iTexValueY", &m_iTexSizeY, sizeof(_int));

    m_pShaderCom->Begin(6);

    m_pVIBufferCom[m_iIndex]->Bind_Resources();
    m_pVIBufferCom[m_iIndex]->Render();

    return S_OK;
}

HRESULT CScreen_Blood_Particle::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_PointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_0"),
        TEXT("Com_VIBuffer0"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[0]), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_1"),
        TEXT("Com_VIBuffer1"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[1]), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_2"),
        TEXT("Com_VIBuffer2"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[2]), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_3"),
        TEXT("Com_VIBuffer3"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[3]), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_4"),
        TEXT("Com_VIBuffer4"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[4]), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Bloode_Splatter"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;
    return S_OK;
}

CScreen_Blood_Particle* CScreen_Blood_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen_Blood_Particle* pInstance = new CScreen_Blood_Particle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CScreen_Blood_Particle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CScreen_Blood_Particle::Clone(void* pArg)
{
    CScreen_Blood_Particle* pInstance = new CScreen_Blood_Particle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CScreen_Blood_Particle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CScreen_Blood_Particle::Free()
{
    __super::Free();

    for(_int i = 0; i < 5; ++i)
        Safe_Release(m_pVIBufferCom[i]);
}
