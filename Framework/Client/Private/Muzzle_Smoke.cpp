#include "pch.h"
#include "Muzzle_Smoke.h"
#include "GameInstance.h"

CMuzzle_Smoke::CMuzzle_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CMuzzle_Smoke::CMuzzle_Smoke(const CMuzzle_Smoke& Prototype) : CPartObject( Prototype )
{
}

HRESULT CMuzzle_Smoke::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMuzzle_Smoke::Initialize(void* pArg)
{
    m_iTexSizeX = 8;
    m_iTexSizeY = 8;


    MUZZLE_SMOKE_DATA* pDesc = static_cast<MUZZLE_SMOKE_DATA*>(pArg);

    m_BlackBoard = pDesc->m_BlackBoard;
    Safe_AddRef(m_BlackBoard);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CMuzzle_Smoke::Priority_Update(_float fTimeDelta)
{
}

void CMuzzle_Smoke::Update(_float fTimeDelta)
{


}

void CMuzzle_Smoke::Late_Update(_float fTimeDelta)
{
    m_fCount += fTimeDelta * 0.3f;

    m_pTransformCom->Go_Right(fTimeDelta * 3.f);
    m_pTransformCom->Turn(_vector{ 0.f, 0.f, 1.f, 0.f }, fTimeDelta * XMConvertToRadians(15.f));
    m_pTransformCom->Scale(_float3{ 1.f + (2.f * m_fCount), 1.f + (2.f * m_fCount), 1.f });
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
    m_pVIBufferCom->Sprite_Sort(fTimeDelta* 0.3f, XMLoadFloat4x4(&m_CombinedWorldMatrix));


    if (m_fCount >= 1.f)
    {
        m_fCount = 0;
        m_pVIBufferCom->Reseet();
        m_BlackBoard->Set_Data().isEndEffect2 = true;
        m_pTransformCom->Set_State(STATE::POSITION, _vector{ 0.f, 0.f, 0.f, 1.f });
        m_pTransformCom->Rotation(_vector{ 0.f, 0.f, 1.f, 0.f }, XMConvertToRadians(0.f));
    }
    else
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONLIGHT, this)))
            return;
    }
}

HRESULT CMuzzle_Smoke::Render()
{
    m_pShaderCom->Bind_RawValue("g_iTexValueX", &m_iTexSizeX, sizeof(_int));
    m_pShaderCom->Bind_RawValue("g_iTexValueY", &m_iTexSizeY, sizeof(_int));
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Bind_Resources();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CMuzzle_Smoke::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_PointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Muzzle_Smoke"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Muzzle_Smoke"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMuzzle_Smoke::Bind_ShaderResources()
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

CMuzzle_Smoke* CMuzzle_Smoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMuzzle_Smoke* pInstance = new CMuzzle_Smoke(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CMuzzle_Smoke"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMuzzle_Smoke::Clone(void* pArg)
{
    CMuzzle_Smoke* pInstance = new CMuzzle_Smoke(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CMuzzle_Smoke"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMuzzle_Smoke::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);

    Safe_Release(m_BlackBoard);
}
