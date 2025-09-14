#include "pch.h"
#include "Blood_Effect.h"
#include "Blood_Smoke.h"
#include "Blood_Splatter.h"

CBlood_Effect::CBlood_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingContainer{ pDevice, pContext }
{
}

CBlood_Effect::CBlood_Effect(const CBlood_Effect& Prototype) : CPoolingContainer(Prototype)
{
}

HRESULT CBlood_Effect::Initialize_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Blude_Smoke"),
        CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Muzzle/Smoke_Effect_%d.png"), 1))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Blude_Splatter"),
        CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Blood/Blood_Effect_%d.png"), 1))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blode_Smoke"),
        CBlood_Smoke::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blode_Splatter"),
        CBlood_Splatter::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBlood_Effect::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_BlackBoard()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CBlood_Effect::Priority_Update(_float fTimeDelta)
{

}

void CBlood_Effect::Update(_float fTimeDelta)
{
}

void CBlood_Effect::Late_Update(_float fTimeDelta)
{
    m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));

    if (m_BlackBoard->Get_Data().isEndEffect1 == false)
        m_PartObjects.at(TEXT("Part_Smoke"))->Late_Update(fTimeDelta);

    if (m_BlackBoard->Get_Data().isEndEffect2 == false)
        m_PartObjects.at(TEXT("Part_Splatter"))->Late_Update(fTimeDelta);

    if (m_BlackBoard->Get_Data().isEndEffect1 == true && m_BlackBoard->Get_Data().isEndEffect2 == true)
        SetDead();
}

HRESULT CBlood_Effect::Render()
{
    return S_OK;
}

HRESULT CBlood_Effect::Initialize_Pool(void* pArg)
{
    BLODE_EFFECT_INIT* pDesc = static_cast<BLODE_EFFECT_INIT*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPos, 1.f));

    return S_OK;
}

void CBlood_Effect::Return_Pool()
{
    m_bIsDead = false;
    m_BlackBoard->Set_Data().fEffectTime = 0.f;
    m_BlackBoard->Set_Data().isEndEffect1 = false;
    m_BlackBoard->Set_Data().isEndEffect2 = false;
}

HRESULT CBlood_Effect::Ready_PartObjects()
{
    CBlood_Smoke::BLODE_SMOKE_DATA SmokeDesc;
    SmokeDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    SmokeDesc.m_BlackBoard = m_BlackBoard;
    SmokeDesc.fRotationPerSec = 1.f;
    SmokeDesc.fSpeedPerSec = 1.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Smoke"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blode_Smoke"), &SmokeDesc)))
        return E_FAIL;

    CBlood_Splatter::BLODE_SPLATTER_DATA SplatterDesc;
    SplatterDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    SplatterDesc.m_BlackBoard = m_BlackBoard;
    SplatterDesc.fRotationPerSec = 1.f;
    SplatterDesc.fSpeedPerSec = 1.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Splatter"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blode_Splatter"), &SplatterDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBlood_Effect::Ready_BlackBoard()
{
    m_BlackBoard = CBlackBoard<BLODE_EFFECT_DATA>::Create();

    m_BlackBoard->Set_Data().fEffectTime = 0.f;
    m_BlackBoard->Set_Data().isEndEffect1 = false;
    m_BlackBoard->Set_Data().isEndEffect2 = false;

    return S_OK;
}

CBlood_Effect* CBlood_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBlood_Effect* pInstance = new CBlood_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBlood_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBlood_Effect::Clone(void* pArg)
{
    CBlood_Effect* pInstance = new CBlood_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBlood_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBlood_Effect::Free()
{
    __super::Free();
    Safe_Release(m_BlackBoard);
}
