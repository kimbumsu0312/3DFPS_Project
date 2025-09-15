#include "pch.h"
#include "Spark_Effect.h"
#include "Spark_Particle.h"

CSpark_Effect::CSpark_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingContainer{ pDevice, pContext }
{
}

CSpark_Effect::CSpark_Effect(const CSpark_Effect& Prototype) : CPoolingContainer(Prototype)
{
}

HRESULT CSpark_Effect::Initialize_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Spark"),
        CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Spark/Spark_%d.png"), 1))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Spark_Particle"),
        CSpark_Particle::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CSpark_Effect::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_BlackBoard()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CSpark_Effect::Priority_Update(_float fTimeDelta)
{

}

void CSpark_Effect::Update(_float fTimeDelta)
{
}

void CSpark_Effect::Late_Update(_float fTimeDelta)
{
    m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));

    if (m_BlackBoard->Get_Data().isEndEffect1 == false)
        m_PartObjects.at(TEXT("Part_Paricle"))->Late_Update(fTimeDelta);

    if (m_BlackBoard->Get_Data().isEndEffect1 == true )
        SetDead();
}

HRESULT CSpark_Effect::Render()
{
    return S_OK;
}

HRESULT CSpark_Effect::Initialize_Pool(void* pArg)
{
    SPARK_EFFECT_INIT* pDesc = static_cast<SPARK_EFFECT_INIT*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(pDesc->vPos, 1.f));

    return S_OK;
}

void CSpark_Effect::Return_Pool()
{
    m_bIsDead = false;
    m_BlackBoard->Set_Data().fEffectTime = 0.f;
    m_BlackBoard->Set_Data().isEndEffect1 = false;
}

HRESULT CSpark_Effect::Ready_PartObjects()
{
    CSpark_Particle::SPARK_PARTICLE_DATA SplatterDesc;
    SplatterDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    SplatterDesc.m_BlackBoard = m_BlackBoard;
    SplatterDesc.fRotationPerSec = 1.f;
    SplatterDesc.fSpeedPerSec = 1.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Paricle"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Spark_Particle"), &SplatterDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSpark_Effect::Ready_BlackBoard()
{
    m_BlackBoard = CBlackBoard<SPARK_EFFECT_DATA>::Create();

    m_BlackBoard->Set_Data().fEffectTime = 0.f;
    m_BlackBoard->Set_Data().isEndEffect1 = false;
    return S_OK;
}

CSpark_Effect* CSpark_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSpark_Effect* pInstance = new CSpark_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSpark_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSpark_Effect::Clone(void* pArg)
{
    CSpark_Effect* pInstance = new CSpark_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSpark_Effect"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSpark_Effect::Free()
{
    __super::Free();
    Safe_Release(m_BlackBoard);
}
