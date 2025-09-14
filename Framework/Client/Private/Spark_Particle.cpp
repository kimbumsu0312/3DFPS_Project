#include "pch.h"
#include "Spark_Particle.h"
#include "GameInstance.h"

CSpark_Particle::CSpark_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CSpark_Particle::CSpark_Particle(const CSpark_Particle& Prototype) : CPartObject(Prototype)
{
}

HRESULT CSpark_Particle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSpark_Particle::Initialize(void* pArg)
{
    m_iTexSizeX = 4;
    m_iTexSizeY = 4;
    m_fCount = 0.f;
    SPARK_PARTICLE_DATA* pDesc = static_cast<SPARK_PARTICLE_DATA*>(pArg);

    m_BlackBoard = pDesc->m_BlackBoard;
    Safe_AddRef(m_BlackBoard);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CSpark_Particle::Priority_Update(_float fTimeDelta)
{
}

void CSpark_Particle::Update(_float fTimeDelta)
{


}

void CSpark_Particle::Late_Update(_float fTimeDelta)
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
        m_BlackBoard->Set_Data().isEndEffect1 = true;
        m_pTransformCom->Set_State(STATE::POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });
        m_pTransformCom->Rotation(_vector{ 0.f, 0.f, 1.f, 0.f }, XMConvertToRadians(0.f));
    }
    else
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONLIGHT, this)))
            return;
    }
}

HRESULT CSpark_Particle::Render()
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

HRESULT CSpark_Particle::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_PointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Spark"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Spark"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSpark_Particle::Bind_ShaderResources()
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

CSpark_Particle* CSpark_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSpark_Particle* pInstance = new CSpark_Particle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSpark_Particle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSpark_Particle::Clone(void* pArg)
{
    CSpark_Particle* pInstance = new CSpark_Particle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CSpark_Particle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSpark_Particle::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);

    Safe_Release(m_BlackBoard);
}
