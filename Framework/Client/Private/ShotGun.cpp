#include "pch.h"
#include "ShotGun.h"

CShotGun::CShotGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CWeaponObject{ pDevice, pContext }
{
}

CShotGun::CShotGun(const CShotGun& Prototype) : CWeaponObject(Prototype)
{
}

HRESULT CShotGun::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CShotGun::Initialize(void* pArg)
{
    m_AnimTag = "Idle_Loop";
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-0.09f, -0.03f, -0.035f, 1.f));
    m_pTransformCom->Rotation_All(_float3{ XMConvertToRadians(-78.f), XMConvertToRadians(0.f), XMConvertToRadians(-90.f) });

    m_pTransformCom->Scale(_float3{ 1.f, 1.f, 1.f });
    return S_OK;
}

void CShotGun::Priority_Update(_float fTimeDelta)
{
}

void CShotGun::Update(_float fTimeDelta)
{
    if (*m_pCulStateTag == TEXT("Attack"))
    {
        m_fAccTime += fTimeDelta;
        m_pAnimCom->Player_Animation(0, "Aimshot", false, m_pModelCom, fTimeDelta, 0);

        if (m_fAccTime > 1.35f && !m_IsSounde)
        {
            m_IsSounde = true;
            m_pGameInstance->StopSound(ENUM_CLASS(SOUND_CHANNEL::PLAYER));
            m_pGameInstance->PlaySoundW(TEXT("ShotGun_Reload_2.wav"), ENUM_CLASS(SOUND_CHANNEL::PLAYER), g_fBGMVolume - 0.2f);
        }
    }
    else if (*m_pCulStateTag == TEXT("Reload"))
    {
        m_fAccTime = 0.f;
        m_IsSounde = false;
        m_bEffect = false;
        if (m_isReload == false)
        {
            m_iReloadStack = 0;
            m_isReload = true;
        }
        if (m_isReload)
        {
            if (m_iReloadStack == 0)
            {
                if (m_pAnimCom->Player_Animation(0, "Reload_Start", false, m_pModelCom, fTimeDelta, 0))
                    m_iReloadStack = 1;
            }
            else if (m_iReloadStack == 1)
            {
                if (m_pAnimCom->Player_Animation(0, "Reload_Loop", false, m_pModelCom, fTimeDelta, 0, false))
                    m_iReloadStack = 2;
            }
            else if(m_iReloadStack == 2)
            {
                if (m_pAnimCom->Player_Animation(0, "Reload_End", false, m_pModelCom, fTimeDelta, 0, false))
                    m_iReloadStack = 3;
            }
        }
    }
    else
    {
        m_fAccTime = 0.f;
        m_IsSounde = false;
        m_bEffect = false;
        m_isReload = false;
        m_pAnimCom->Player_Animation(0, "Idle_Loop", true, m_pModelCom, fTimeDelta, 0);
    }
}

void CShotGun::Late_Update(_float fTimeDelta)
{
    _matrix     BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);
    _matrix     ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
    _matrix     WorldMatrix = m_pTransformCom->Get_WorldMatrix();

    for (size_t i = 0; i < 3; i++)
    {
        BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);
    }
    XMStoreFloat4x4(&m_CombinedWorldMatrix, WorldMatrix * BoneMatrix * ParentMatrix);

    if (*m_pCulStateTag == TEXT("Attack"))
    {
        if (!m_bEffect)
        {
            m_bEffect = true;
            CMuzzle_Effect::MUZZLE_EFFECT_INIT Desc;

            _vector vScale, vWorldRot, vWorldTrans;
            XMMatrixDecompose(&vScale, &vWorldRot, &vWorldTrans, XMLoadFloat4x4(&m_CombinedWorldMatrix));

            _vector vLook = { m_CombinedWorldMatrix._31,m_CombinedWorldMatrix._32, m_CombinedWorldMatrix._33, 0.f };

            _vector vOffset = { 0.f, 0.f , m_fRange, 1.f };
            vOffset = XMVector3Rotate(vOffset, vWorldRot);


            Desc.vPos = vWorldTrans + vOffset;

            m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Muzzle_ShotGun"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &Desc);
        }
    }
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;

}

HRESULT CShotGun::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint           iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
            continue;

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
            continue;

        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, 1, 0)))
            m_pShaderCom->Begin(0);
        else
            m_pShaderCom->Begin(2);

        m_pModelCom->Render(i);
    }



    return S_OK;
}

HRESULT CShotGun::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_ShotGun_Weapon"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
        return E_FAIL;

    CAnimatio_Controller::ANIMTION_DESC Desc;
    Desc.szFile_Path = "../Bin/Resources/Models/ShotGun/ShotGunAnim.Json";
    Desc.szCulAnimName = m_AnimTag;
    Desc.iAnimIndex = 0;
    Desc.pModel = m_pModelCom;

    Desc.IsLoop = true;
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
        TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
        return E_FAIL;
    return S_OK;
}

HRESULT CShotGun::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;



    return S_OK;
}

CShotGun* CShotGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CShotGun* pInstance = new CShotGun(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CShotGun"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CShotGun::Clone(void* pArg)
{
    CShotGun* pInstance = new CShotGun(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CShotGun"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CShotGun::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pAnimCom);
}
