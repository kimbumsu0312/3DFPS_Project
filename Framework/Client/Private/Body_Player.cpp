#include "pch.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Player.h"
CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject { pDevice, pContext}
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{

    BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
    
    m_pState = pDesc->pState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pModelCom->Set_Animations(0, true);

    m_iRootLodeIndex = 36;
    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
}

void CBody_Player::Update(_float fTimeDelta)
{
    //Crouch_loop
    switch (*m_pState)
    {
    case ENUM_CLASS(CPlayer::PLAYER_STATE::IDLE):
        m_pAnimCom->Player_Animation("Hand_Idle", m_pModelCom, fTimeDelta, m_iRootLodeIndex, m_pTransformCom);
        break;
    case ENUM_CLASS(CPlayer::PLAYER_STATE::JOG_F):
        m_pAnimCom->Player_Animation("Hand_Jog_Front", m_pModelCom, fTimeDelta, m_iRootLodeIndex, m_pTransformCom);
        break;
    case ENUM_CLASS(CPlayer::PLAYER_STATE::JOG_R):
        m_pAnimCom->Player_Animation("Hand_Jog_Right", m_pModelCom, fTimeDelta , m_iRootLodeIndex, m_pTransformCom);
        break;
    case ENUM_CLASS(CPlayer::PLAYER_STATE::JOG_L):
        m_pAnimCom->Player_Animation("Hand_Jog_Left", m_pModelCom, fTimeDelta, m_iRootLodeIndex, m_pTransformCom);
        break;
    case ENUM_CLASS(CPlayer::PLAYER_STATE::WALK_B):
        m_pAnimCom->Player_Animation("Hand_Walk_Back", m_pModelCom, fTimeDelta, m_iRootLodeIndex, m_pTransformCom);
        break;
    }
}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CBody_Player::Render()
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

HRESULT CBody_Player::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Player"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/Player/PlayerAnim.Json";
    Desc.szCulAnimName = "Hand_Idle";
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;
 
    return S_OK;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
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

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBody_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();
    //m_pState = nullptr;
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);
}
