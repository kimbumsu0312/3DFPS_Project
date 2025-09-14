#include "pch.h"
#include "Muzzle_Effect.h"
#include "Muzzle.h"
#include "Muzzle_Smoke.h"

CMuzzle_Effect::CMuzzle_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingContainer{pDevice, pContext}
{
}

CMuzzle_Effect::CMuzzle_Effect(const CMuzzle_Effect& Prototype) : CPoolingContainer(Prototype)
{
}

HRESULT CMuzzle_Effect::Initialize_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Muzzle"),
        CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Muzzle/Muzzle_Effect_%d.png"), 1))))
        return E_FAIL;

   if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Muzzle_Smoke"),
       CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Muzzle/Smoke_Effect_%d.png"), 1))))
       return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle"),
        CMuzzle::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Smoke"),
        CMuzzle_Smoke::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMuzzle_Effect::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_BlackBoard()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CMuzzle_Effect::Priority_Update(_float fTimeDelta)
{
    
}

void CMuzzle_Effect::Update(_float fTimeDelta)
{
}

void CMuzzle_Effect::Late_Update(_float fTimeDelta)
{
    m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));
    if (m_BlackBoard->Get_Data().isEndEffect2 == false)
       m_PartObjects.at(TEXT("Part_Smoke"))->Late_Update(fTimeDelta);

    if (m_BlackBoard->Get_Data().isEndEffect1 == false)
        m_PartObjects.at(TEXT("Part_Muzzle"))->Late_Update(fTimeDelta);

    if(m_BlackBoard->Get_Data().isEndEffect1 == true && m_BlackBoard->Get_Data().isEndEffect2 == true)
        SetDead();
}

HRESULT CMuzzle_Effect::Render()
{
    return S_OK;
}

HRESULT CMuzzle_Effect::Initialize_Pool(void* pArg)
{
    MUZZLE_EFFECT_INIT* pDesc = static_cast<MUZZLE_EFFECT_INIT*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPos, 1.f));

    return S_OK;
}

void CMuzzle_Effect::Return_Pool()
{
    m_bIsDead = false;
    m_BlackBoard->Set_Data().fEffectTime = 0.f;
    m_BlackBoard->Set_Data().isEndEffect1 = false;
    m_BlackBoard->Set_Data().isEndEffect2 = false;
}

HRESULT CMuzzle_Effect::Ready_PartObjects()
{
    CMuzzle::MUZZLE_DATA Desc;
    Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    Desc.m_BlackBoard = m_BlackBoard;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Muzzle"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle"),&Desc)))
        return E_FAIL;

    CMuzzle_Smoke::MUZZLE_SMOKE_DATA SmokeDesc;
    SmokeDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    SmokeDesc.m_BlackBoard = m_BlackBoard;
    SmokeDesc.fRotationPerSec = 1.f;
    SmokeDesc.fSpeedPerSec = 1.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Smoke"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Smoke"), &SmokeDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMuzzle_Effect::Ready_BlackBoard()
{
    m_BlackBoard = CBlackBoard<MUZZLE_EFFECT_DATA>::Create();

    m_BlackBoard->Set_Data().fEffectTime = 0.f;
    m_BlackBoard->Set_Data().isEndEffect1 = false;
    m_BlackBoard->Set_Data().isEndEffect2 = false;

    return S_OK;
}

CMuzzle_Effect* CMuzzle_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMuzzle_Effect* pInstance = new CMuzzle_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CMuzzle_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMuzzle_Effect::Clone(void* pArg)
{
    CMuzzle_Effect* pInstance = new CMuzzle_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CMuzzle_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMuzzle_Effect::Free()
{
    __super::Free();
    Safe_Release(m_BlackBoard);
}
