#include "pch.h"
#include "Body_Alcina.h"
#include "GameInstance.h"

CBody_Alcina::CBody_Alcina(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CBody_Alcina::CBody_Alcina(const CBody_Alcina& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBody_Alcina::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Alcina::Initialize(void* pArg)
{
    BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
    m_pBlackBoard = pDesc->pBalckBoard;
    Safe_AddRef(m_pBlackBoard);

    m_iRootLodeIndex = 40;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CBody_Alcina::Priority_Update(_float fTimeDelta)
{
}

void CBody_Alcina::Update(_float fTimeDelta)
{
    CAlcina::ALCHINA_DATA& Alchina_Data = m_pBlackBoard->Set_Data();

    (*Alchina_Data.bIsAnimFinsh) = m_pAnimCom->Player_Animation((*Alchina_Data.iAnimState), (*Alchina_Data.szAnimTag), (*Alchina_Data.bIsAnimLoop), m_pModelCom, fTimeDelta, m_iRootLodeIndex);

    Update_CombinedMatrix();
}

void CBody_Alcina::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CBody_Alcina::Render()
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

_float4x4* CBody_Alcina::Get_BoneMatrix(const _wstring pBoneName)
{
    return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_float3* CBody_Alcina::Get_MovePos()
{
    return m_pModelCom->Get_PtrMovePos();
}

_float4* CBody_Alcina::Get_MoveRot()
{
    return m_pModelCom->Get_PtrMoveRot();;

}

HRESULT CBody_Alcina::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Alcina"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/Boss/Alcina/AlcinaAnim.Json";
    Desc.szCulAnimName = *m_pBlackBoard->Get_Data().szAnimTag;
    Desc.iAnimIndex = *m_pBlackBoard->Get_Data().iAnimState;
    Desc.pModel = m_pModelCom;

    Desc.IsLoop = true;
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Alcina::Bind_ShaderResources()
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

CBody_Alcina* CBody_Alcina::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Alcina* pInstance = new CBody_Alcina(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Alcina::Clone(void* pArg)
{
    CBody_Alcina* pInstance = new CBody_Alcina(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Alcina"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Alcina::Free()
{
    __super::Free();
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);

    Safe_Release(m_pBlackBoard);
}
