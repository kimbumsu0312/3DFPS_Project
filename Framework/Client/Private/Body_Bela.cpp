#include "pch.h"
#include "Body_Bela.h"
#include "GameInstance.h"

CBody_Bela::CBody_Bela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CBody_Bela::CBody_Bela(const CBody_Bela& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBody_Bela::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Bela::Initialize(void* pArg)
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

    m_iRootLodeIndex = 32;

    return S_OK;
}

void CBody_Bela::Priority_Update(_float fTimeDelta)
{
}

void CBody_Bela::Update(_float fTimeDelta)
{
    *m_pIsAnimFinsh = m_pAnimCom->Player_Animation(*m_pAnimState, *m_pAnimTag, *m_pIsAnimLoop, m_pModelCom, fTimeDelta, m_iRootLodeIndex);

    Update_CombinedMatrix();
}

void CBody_Bela::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CBody_Bela::Render()
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

_float4x4* CBody_Bela::Get_BoneMatrix(const _wstring pBoneName)
{
    return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_float3* CBody_Bela::Get_MovePos()
{
    return m_pModelCom->Get_PtrMovePos();
}

_float4* CBody_Bela::Get_MoveRot()
{
    return m_pModelCom->Get_PtrMoveRot();;
}

HRESULT CBody_Bela::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Bela"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/Boss/Bela/BelaAnim.Json";
    Desc.szCulAnimName = *m_pAnimTag;
    Desc.iAnimIndex = *m_pAnimState;
    Desc.pModel = m_pModelCom;

    Desc.IsLoop = true;
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Bela::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    return S_OK;
}

CBody_Bela* CBody_Bela::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Bela* pInstance = new CBody_Bela(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Bela::Clone(void* pArg)
{
    CBody_Bela* pInstance = new CBody_Bela(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Bela::Free()
{
    __super::Free();
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);
}
