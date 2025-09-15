#include "pch.h"
#include "Blood_Splatter.h"
#include "GameInstance.h"

CBlood_Splatter::CBlood_Splatter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CBlood_Splatter::CBlood_Splatter(const CBlood_Splatter& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBlood_Splatter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlood_Splatter::Initialize(void* pArg)
{
    m_iTexSizeX = 4;
    m_iTexSizeY = 4;
    m_fCount = 0.f;
    BLODE_SPLATTER_DATA* pDesc = static_cast<BLODE_SPLATTER_DATA*>(pArg);

    m_BlackBoard = pDesc->m_BlackBoard;
    Safe_AddRef(m_BlackBoard);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CBlood_Splatter::Priority_Update(_float fTimeDelta)
{
}

void CBlood_Splatter::Update(_float fTimeDelta)
{


}

void CBlood_Splatter::Late_Update(_float fTimeDelta)
{
    m_fCount += fTimeDelta;

 //   m_pTransformCom->Go(fTimeDelta * 0.5f);

   // m_pTransformCom->Turn(_vector{ 0.f, 0.f, 1.f, 0.f }, fTimeDelta * XMConvertToRadians(15.f));
    //m_pTransformCom->Scale(_float3{ 1.f * m_fCount, 1.f * m_fCount, 1.f });
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
    m_pVIBufferCom->Spread(fTimeDelta);


    if (m_fCount >= 1.f)
    {
        m_fCount = 0.f;
        m_pVIBufferCom->Reseet();
        m_BlackBoard->Set_Data().isEndEffect2 = true;
        m_pTransformCom->Set_State(STATE::POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });
        m_pTransformCom->Rotation(_vector{ 0.f, 0.f, 1.f, 0.f }, XMConvertToRadians(0.f));
    }
    else
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::EFFECT, this)))
            return;
    }
}

HRESULT CBlood_Splatter::Render()
{
    m_pShaderCom->Bind_RawValue("g_iTexValueX", &m_iTexSizeX, sizeof(_int));
    m_pShaderCom->Bind_RawValue("g_iTexValueY", &m_iTexSizeY, sizeof(_int));
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(4);

    m_pVIBufferCom->Bind_Resources();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBlood_Splatter::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_PointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Blode_Splatter"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Blude_Splatter"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBlood_Splatter::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_DiffuseTexture", 0)))
        return E_FAIL;

    return S_OK;
}

CBlood_Splatter* CBlood_Splatter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBlood_Splatter* pInstance = new CBlood_Splatter(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBlood_Splatter"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBlood_Splatter::Clone(void* pArg)
{
    CBlood_Splatter* pInstance = new CBlood_Splatter(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBlood_Splatter"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBlood_Splatter::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);

    Safe_Release(m_BlackBoard);
}
