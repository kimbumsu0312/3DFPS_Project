#include "pch.h"
#include "Camera_Player.h"

CCamera_Player::CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CCamera{ pDevice, pContext }
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& Prototype) : CCamera(Prototype)
{
}

HRESULT CCamera_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Player::Initialize(void* pArg)
{
    CAMERA_PLAYER_DESC* pDesc = static_cast<CAMERA_PLAYER_DESC*>(pArg);

    m_fMouseSensor = pDesc->fMouseSensor;
    m_pSocketMatrix1 = pDesc->pSocketMatrix1;
    m_pSocketMatrix2 = pDesc->pSocketMatrix2;
    m_pParentMatrix = pDesc->pParentMatrix;
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    m_DefultWorldMatrix = m_pTransformCom->Get_WorldMatrix();
    return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyHold(DIK_Q))
        return;
        __super::Update_PipeLines();
}

void CCamera_Player::Update(_float fTimeDelta)
{

}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
    Update_CamraPos();
}

HRESULT CCamera_Player::Render()
{
    return S_OK;
}

void CCamera_Player::Zoom_In(_float fTimeDelta)
{
    if (!m_bIsZoomIn)
        return;

    if (XMConvertToRadians(m_fMoveFovy) < m_fFovy)
        m_fFovy -= XMConvertToRadians(m_fZoomSpeed * fTimeDelta);
    else
    {
        m_fFovy = XMConvertToRadians(m_fMoveFovy);
        m_bIsZoomIn = false;
    }
}

void CCamera_Player::Zoom_Out(_float fTimeDelta)
{
    if (!m_bIsZoomout)
        return;

    if (XMConvertToRadians(m_fMoveFovy) > m_fFovy)
        m_fFovy += XMConvertToRadians(m_fZoomSpeed) * fTimeDelta;
    else
    {
        m_fFovy = XMConvertToRadians(m_fMoveFovy);
        m_bIsZoomout = false;
    }
}

void CCamera_Player::Update_CamraPos()
{
    _matrix     MainBoneMatrix = XMLoadFloat4x4(m_pSocketMatrix1);
    _matrix     SubBoneMatrix = XMLoadFloat4x4(m_pSocketMatrix2);
    _matrix     ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
    _matrix     RotaionY = XMMatrixRotationY(XMConvertToRadians(180.f));
    RotaionY.r[3] = { 0.f, 0.f, -0.f ,1.f };
    for (size_t i = 0; i < 3; i++)
    {
        MainBoneMatrix.r[i] = XMVector3Normalize(MainBoneMatrix.r[i]);
        SubBoneMatrix.r[i] = XMVector3Normalize(SubBoneMatrix.r[i]);
       
    }

    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_DefultWorldMatrix * MainBoneMatrix * XMLoadFloat4x4(m_pParentMatrix));
    m_pTransformCom->Set_WorldMatrix(m_CombinedWorldMatrix);
}

CCamera_Player* CCamera_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Player* pInstance = new CCamera_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CCamera_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
    CCamera_Player* pInstance = new CCamera_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CCamera_Player"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCamera_Player::Free()
{
    __super::Free();
    
}
