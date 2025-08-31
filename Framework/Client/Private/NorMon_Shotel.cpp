#include "pch.h"
#include "Normon_Shotel.h"
#include "Player_Manager.h"

CNormon_Shotel::CNormon_Shotel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CWeaponObject{ pDevice, pContext }
{
}

CNormon_Shotel::CNormon_Shotel(const CNormon_Shotel& Prototype) : CWeaponObject(Prototype)
{
}

HRESULT CNormon_Shotel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNormon_Shotel::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scale(_float3{ 1.f, 1.f, 1.f });
    return S_OK;
}

void CNormon_Shotel::Priority_Update(_float fTimeDelta)
{
}

void CNormon_Shotel::Update(_float fTimeDelta)
{

    _matrix     BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);
    _matrix     ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
    _matrix     WorldMatrix = m_pTransformCom->Get_WorldMatrix();

    for (size_t i = 0; i < 3; i++)
    {
        BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);
    }
    XMStoreFloat4x4(&m_CombinedWorldMatrix, WorldMatrix * BoneMatrix * ParentMatrix);

    m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));
}

void CNormon_Shotel::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CNormon_Shotel::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint           iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
            continue;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }
#ifdef _DEBUG
    m_pColliderCom->Render();
#endif // DEBUG

    return S_OK;
}

HRESULT CNormon_Shotel::Add_Collider()
{
    if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom)))
        return E_FAIL;
    return S_OK;
}

void CNormon_Shotel::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
    switch (TargetCollision.iObjType)
    {
    case ENUM_CLASS(OBJECT_TYPE::PLAYER):
        CPlayer_Manager::GetInstance()->Player_Hp(-25);
        break;

    }
}

HRESULT CNormon_Shotel::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Shotel"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
    OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::WEAPON);
    OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::WEAPON);
    OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
    OBBDesc.vExtents = _float3(0.05f, 0.1f, 0.4f);
    OBBDesc.vCenter = _float3(0.f, 0.f, -0.2f);

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CNormon_Shotel::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

CNormon_Shotel* CNormon_Shotel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormon_Shotel* pInstance = new CNormon_Shotel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CNormon_Shotel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormon_Shotel::Clone(void* pArg)
{
    CNormon_Shotel* pInstance = new CNormon_Shotel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CNormon_Shotel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormon_Shotel::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderCom);
}
