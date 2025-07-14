#include "pch.h"
#include "Player_Hp_Beat.h"

CPlayer_Hp_Beat::CPlayer_Hp_Beat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CPlayer_Hp_Beat::CPlayer_Hp_Beat(const CPlayer_Hp_Beat& Prototype) : CUIObject(Prototype)
{
}

HRESULT CPlayer_Hp_Beat::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CPlayer_Hp_Beat::Initialize(void* pArg)
{
    m_fMoveSpeed = 3.f;
    if (pArg == nullptr)
        return E_FAIL;
    m_vColor = { 0.f, 0.6f, 0.f, 0.7f };
    m_fMovePos = 0.f;
    
    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);
    m_fMoveMaxX = Desc->OffsetX;
    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_pGameInstance->Subscribe<Event_Player_Hp_Set>([&](const Event_Player_Hp_Set& e) {m_vColor = e.Color; });

    return S_OK;
}

void CPlayer_Hp_Beat::Priority_Update(_float fTimeDelta)
{

}

void CPlayer_Hp_Beat::Update(_float fTimeDelta)
{  
   
    m_fMovePos += m_fMoveMaxX * 0.1f * (m_fMoveSpeed * fTimeDelta);
    if (m_fMovePos >= m_fMoveMaxX * 0.7f)
        m_fMovePos = -m_fMoveMaxX * 0.5f;
}

void CPlayer_Hp_Beat::Late_Update(_float fTimeDelta)
{
     if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CPlayer_Hp_Beat::Render()
{
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));
    m_pTransformCom->Scale(_float3(m_vSize.x, m_vSize.y, 1.f));
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPos.x - (m_vSize.x * 0.5f) + m_fMovePos - g_iWinSizeX * 0.5f,
        -m_vPos.y + g_iWinSizeY * 0.5f, 0.0f, 1.0f));
    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");

    if (FAILED(m_pShaderCom->Bind_RawValue("g_MinUV", &m_vMinUV, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxUV", &m_vMaxUV, sizeof(_float2))))
        return E_FAIL;

    m_pShaderCom->Begin(7);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CPlayer_Hp_Beat::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CPlayer_Hp_Beat* CPlayer_Hp_Beat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer_Hp_Beat* pInstance = new CPlayer_Hp_Beat(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp_Beat"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer_Hp_Beat::Clone(void* pArg)
{
    CPlayer_Hp_Beat* pInstance = new CPlayer_Hp_Beat(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp_Beat"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer_Hp_Beat::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
