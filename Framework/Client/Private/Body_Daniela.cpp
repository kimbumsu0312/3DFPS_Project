#include "pch.h"
#include "Body_Daniela.h"
#include "GameInstance.h"

CBody_Daniela::CBody_Daniela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CBody_Daniela::CBody_Daniela(const CBody_Daniela& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBody_Daniela::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Daniela::Initialize(void* pArg)
{

    BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);

    m_BlackBoard = pDesc->BlackBoard;
    Safe_AddRef(m_BlackBoard);

    m_iRootLodeIndex = 32;
    
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CBody_Daniela::Priority_Update(_float fTimeDelta)
{
}

void CBody_Daniela::Update(_float fTimeDelta)
{
    CDaniela::DANIELA_DATA& pData = m_BlackBoard->Set_Data();

    *pData.bIsAnimFinsh = m_pAnimCom->Player_Animation(*pData.iAnimState, *pData.szAnimTag, *pData.bIsAnimLoop, m_pModelCom, fTimeDelta, m_iRootLodeIndex, pData.isBogan);

    Update_CombinedMatrix();
}

void CBody_Daniela::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CBody_Daniela::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();
    m_pShaderCom->Bind_RawValue("g_fNoiesValue", &m_BlackBoard->Get_Data().fNoies, sizeof(m_BlackBoard->Get_Data().fNoies));
    m_pNoiesTexCom->Bind_Shader_Resource(m_pShaderCom, "g_NoiesTexture", 0);

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
            continue;

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
            continue;
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, 1, 0)))
            m_pShaderCom->Begin(1);
        else
            m_pShaderCom->Begin(3);

        m_pModelCom->Render(i);
    }

    return S_OK;
}

_float4x4* CBody_Daniela::Get_BoneMatrix(const _wstring pBoneName)
{
    return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_float3* CBody_Daniela::Get_MovePos()
{
    return m_pModelCom->Get_PtrMovePos();
}

_float4* CBody_Daniela::Get_MoveRot()
{
    return m_pModelCom->Get_PtrMoveRot();;

}

HRESULT CBody_Daniela::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Noies"),
        TEXT("Com_NoiesTex"), reinterpret_cast<CComponent**>(&m_pNoiesTexCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Daniela"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/Boss/Daniela/DanielaAnim.Json";
    Desc.szCulAnimName = *m_BlackBoard->Get_Data().szAnimTag;
    Desc.iAnimIndex = *m_BlackBoard->Get_Data().iAnimState;
    Desc.pModel = m_pModelCom;

    Desc.IsLoop = true;
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Daniela::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    return S_OK;
}

CBody_Daniela* CBody_Daniela::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Daniela* pInstance = new CBody_Daniela(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Daniela::Clone(void* pArg)
{
    CBody_Daniela* pInstance = new CBody_Daniela(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Daniela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Daniela::Free()
{
    __super::Free();
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);

    Safe_Release(m_BlackBoard);
    Safe_Release(m_pNoiesTexCom);
}
