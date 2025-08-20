#include "pch.h"
#include "Body_NorMon.h"
#include "GameInstance.h"

CBody_NorMon::CBody_NorMon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CBody_NorMon::CBody_NorMon(const CBody_NorMon& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBody_NorMon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_NorMon::Initialize(void* pArg)
{

    BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);

    m_pAnimState = pDesc->pAnimState;
    m_pAnimTag = pDesc->pAnimTag;
    m_pIsAnimFinsh = pDesc->pIsAnimFinsh;
    m_pIsAnimLoop = pDesc->pIsAnimLoop;
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_iRootLodeIndex = 8;

    return S_OK;
}

void CBody_NorMon::Priority_Update(_float fTimeDelta)
{
}

void CBody_NorMon::Update(_float fTimeDelta)
{
    *m_pIsAnimFinsh = m_pAnimCom->Player_Animation(*m_pAnimState, *m_pAnimTag, *m_pIsAnimLoop, m_pModelCom, fTimeDelta, m_iRootLodeIndex);

    Update_CombinedMatrix();
}

void CBody_NorMon::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CBody_NorMon::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
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

_float4x4* CBody_NorMon::Get_BoneMatrix(const _wstring pBoneName)
{
    return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_float3* CBody_NorMon::Get_MovePos()
{
    return m_pModelCom->Get_PtrMovePos();
}

_float4* CBody_NorMon::Get_MoveRot()
{
    return m_pModelCom->Get_PtrMoveRot();;
}

HRESULT CBody_NorMon::Ready_Components()
{
    m_iRootLodeIndex = 11;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Normal_Mon_1"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/Normal_Mon_1/Normal_Mon_1Anim.Json";
    Desc.szCulAnimName = "Idle_Loop";
    Desc.iAnimIndex = 0;
    Desc.pModel = m_pModelCom;

    Desc.IsLoop = true;
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_NorMon::Bind_ShaderResources()
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

CBody_NorMon* CBody_NorMon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_NorMon* pInstance = new CBody_NorMon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_NorMon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_NorMon::Clone(void* pArg)
{
    CBody_NorMon* pInstance = new CBody_NorMon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_NorMon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_NorMon::Free()
{
    __super::Free();
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);
}
