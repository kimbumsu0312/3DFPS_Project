#include "pch.h"
#include "Normon_Halberd.h"
#include "Player_Manager.h"

CNormon_Halberd::CNormon_Halberd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CWeaponObject{ pDevice, pContext }
{
}

CNormon_Halberd::CNormon_Halberd(const CNormon_Halberd& Prototype) : CWeaponObject(Prototype)
{
}

HRESULT CNormon_Halberd::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNormon_Halberd::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scale(_float3{ 1.f, 1.f, 1.f });
    return S_OK;
}

void CNormon_Halberd::Priority_Update(_float fTimeDelta)
{
}

void CNormon_Halberd::Update(_float fTimeDelta)
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

void CNormon_Halberd::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CNormon_Halberd::Render()
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

HRESULT CNormon_Halberd::Add_Collider()
{
    if (FAILED(m_pGameInstance->Add_ColliderCheck(this, m_pColliderCom)))
        return E_FAIL;
    return S_OK;
}

void CNormon_Halberd::OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)
{
    switch (TargetCollision.iObjType)
    {
    case ENUM_CLASS(OBJECT_TYPE::PLAYER):
        CPlayer_Manager::GetInstance()->Player_Hp(-25);
        break;

    }
}

HRESULT CNormon_Halberd::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Halberd"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CBounding_OBB::BOUNDING_OBB_DESC  OBBDesc{};
    OBBDesc.iLayer = ENUM_CLASS(COLLISION_LAYER::WEAPON);
    OBBDesc.iObjType = ENUM_CLASS(OBJECT_TYPE::WEAPON);
    OBBDesc.vAngles = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
    OBBDesc.vExtents = _float3(0.05f, 0.21f, 0.75f);
    OBBDesc.vCenter = _float3(0.f, 0.f, -0.35f);

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CNormon_Halberd::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    return S_OK;
}

CNormon_Halberd* CNormon_Halberd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormon_Halberd* pInstance = new CNormon_Halberd(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CNormon_Halberd"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormon_Halberd::Clone(void* pArg)
{
    CNormon_Halberd* pInstance = new CNormon_Halberd(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CNormon_Halberd"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormon_Halberd::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderCom);
}
