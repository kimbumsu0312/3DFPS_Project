#include "pch.h"
#include "Fly_Effect.h"

CFly_Effect::CFly_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingContainer{pDevice, pContext}
{
}

CFly_Effect::CFly_Effect(const CFly_Effect& Prototype) : CPoolingContainer(Prototype)
{
}

HRESULT CFly_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFly_Effect::Initialize(void* pArg)
{
    FLY_EFFECT_DESC* pDesc = static_cast<FLY_EFFECT_DESC*>(pArg);
    m_isDeadOn = pDesc->isDead;
    m_eFly_Type = pDesc->eType;
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CFly_Effect::Priority_Update(_float fTimeDelta)
{
}

void CFly_Effect::Update(_float fTimeDelta)
{
}

void CFly_Effect::Late_Update(_float fTimeDelta)
{
    m_fAccTime += fTimeDelta;
    switch (m_eFly_Type)
    {
    case Fly_Type::SPREAD:
        if (m_fAccTime > 3.f)
        { 
            m_pModel_InstanceCom->Spread(fTimeDelta * 10.f, false);
        }
        m_pModel_InstanceCom->LocalOffset_Spin(fTimeDelta * 3.f, false);
        break;
    case Fly_Type::SPIN:
        m_pModel_InstanceCom->LocalOffset_Spin(fTimeDelta * 3.f, true);
        break;

    case Fly_Type::RETURN:
        if (m_fAccTime < 3.f)
            m_pModel_InstanceCom->Spread(-fTimeDelta, false);

        m_pModel_InstanceCom->LocalOffset_Spin(fTimeDelta * 3.f, false);
        break;
    }
   
    if (m_isDeadOn && m_fAccTime > 4.5f)
        SetDead();

    //m_pTransformCom->Turn({ 0.f, 1.f, 0.f, 0.f }, fTimeDelta);
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CFly_Effect::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pModel_InstanceCom->Bind_Instance()))
        return E_FAIL;

    _uint           iNumMeshes = m_pModel_InstanceCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModel_InstanceCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
            continue;

        m_pShaderCom->Begin(0);
        m_pModel_InstanceCom->Render(i);
    }

    return S_OK;
}

HRESULT CFly_Effect::Initialize_Pool(void* pArg)
{
    FLY_EFFECT_INIT* pDesc = static_cast<FLY_EFFECT_INIT*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPos, 1.f));
    m_fAccTime = 0.f;
    return S_OK;
}

void CFly_Effect::Return_Pool()
{
    m_pModel_InstanceCom->Reset();
    m_bIsDead = false;
}

void CFly_Effect::Set_Potion(_vector vPos)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(vPos, 1.f));
}

void CFly_Effect::Reset()
{
    m_fAccTime = 0.f;
    m_pModel_InstanceCom->Reset();
}

HRESULT CFly_Effect::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_MeshParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    switch (m_eFly_Type)
    {
    case CFly_Effect::Fly_Type::SPREAD:
        if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Fly_Spread"),
            TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModel_InstanceCom), nullptr)))
            return E_FAIL;
        break;
    case CFly_Effect::Fly_Type::RETURN:
        if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Fly_Spread_Return"),
            TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModel_InstanceCom), nullptr)))
            return E_FAIL;
        break;
    case CFly_Effect::Fly_Type::SPIN:
        if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Fly_Spin"),
            TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModel_InstanceCom), nullptr)))
            return E_FAIL;
        break;

    }


    return S_OK;
}

HRESULT CFly_Effect::Bind_ShaderResources()
{
    m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix");
    
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    return S_OK;
}

void CFly_Effect::Spread_Effect(_float fTimeDelta)
{
}

void CFly_Effect::Spin_Effect(_float fTimeDelta)
{
}

CFly_Effect* CFly_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFly_Effect* pInstance = new CFly_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CFly_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFly_Effect::Clone(void* pArg)
{
    CFly_Effect* pInstance = new CFly_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CFly_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFly_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModel_InstanceCom);
}
