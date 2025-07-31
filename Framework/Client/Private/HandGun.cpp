#include "pch.h"
#include "HandGun.h"
#include "Client_Enum.h"
CHandGun::CHandGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CHandGun::CHandGun(const CHandGun& Prototype) : CPartObject(Prototype)
{
}

HRESULT CHandGun::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHandGun::Initialize(void* pArg)
{
    HANDGUN_DESC* pDesc = static_cast<HANDGUN_DESC*>(pArg);
    m_pParentState = pDesc->pState;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_AnimTag = "Idle";

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-0.015f, -0.03f, -0.01f, 1.f));

    m_pTransformCom->Rotation_All(_float3{ XMConvertToRadians(110.0f), XMConvertToRadians(180.f), XMConvertToRadians(50.f) });
    m_pTransformCom->Scale(_float3{ 1.f, 1.f, 1.f });

    return S_OK;
}

void CHandGun::Priority_Update(_float fTimeDelta)
{
}

void CHandGun::Update(_float fTimeDelta)
{
    if (*m_pParentState == PLAYER_STATE::ATTACK)
    {
        m_pAnimCom->Player_Animation(0, "Aim_Shot", false, m_pModelCom, fTimeDelta, 0);
    }
    else if (*m_pParentState == PLAYER_STATE::RELOAD)
    { 
        m_pAnimCom->Player_Animation(0, "Reload", false, m_pModelCom, fTimeDelta, 0);
    }
    else
    {
        m_pAnimCom->Player_Animation(0, "Idle", true, m_pModelCom, fTimeDelta, 0);
    }

}

void CHandGun::Late_Update(_float fTimeDelta)
{
    _matrix     BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);
    _matrix     ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
    _matrix     WorldMatrix = m_pTransformCom->Get_WorldMatrix();

    for (size_t i = 0; i < 3; i++)
    {
        BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);
    }
    XMStoreFloat4x4(&m_CombinedWorldMatrix, WorldMatrix * BoneMatrix * ParentMatrix);
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CHandGun::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint           iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
            continue;

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
            continue;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }



    return S_OK;
}

HRESULT CHandGun::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_HandGun"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/HandGun/HandGunAnim.json";
    Desc.szCulAnimName = m_AnimTag;
    Desc.iAnimIndex = 0;
    Desc.pModel = m_pModelCom;

    Desc.IsLoop = true;
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;
    return S_OK;
}

HRESULT CHandGun::Bind_ShaderResources()
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

CHandGun* CHandGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHandGun* pInstance = new CHandGun(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CHandGun"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHandGun::Clone(void* pArg)
{
    CHandGun* pInstance = new CHandGun(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CHandGun"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHandGun::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);
}
