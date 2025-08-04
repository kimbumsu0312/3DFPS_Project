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

    //m_pGameInstance->Subscribe<Event_Camera_Zoom>([&](const Event_Camera_Zoom& e) {
    //    switch (e.eState)
    //    {
    //    case CAMERA_STATE::ZOOM_IN:
    //        m_bIsZoomIn = true;
    //        break;
    //    case CAMERA_STATE::ZOOM_OUT:
    //        m_bIsZoomout = true;
    //        break;
    //    case CAMERA_STATE::ZOOM_RESET:
    //        m_fFovy = m_fResetFovy;
    //        m_bIsZoomIn = false;
    //        m_bIsZoomout = false;
    //        return;
    //    }
    //    m_fResetFovy = m_fFovy;
    //    m_fMoveFovy = e.fZoomFov;
    //    m_fZoomSpeed = e.fZoomSpeed;
    //    });
    return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyHold(DIK_Q))
        return;

    Zoom_In(fTimeDelta);
    Zoom_Out(fTimeDelta);
    Update_CameraRot(fTimeDelta);
    Update_CameraPos();
    __super::Update_PipeLines();
}

void CCamera_Player::Update(_float fTimeDelta)
{

}

void CCamera_Player::Late_Update(_float fTimeDelta)
{

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

void CCamera_Player::Update_CameraRot(_float fTimeDelta)
{

}

void CCamera_Player::Update_CameraPos()
{
    _matrix matYaw = XMMatrixRotationY(m_fYaw);     // Y축 (좌우)
    _matrix matPitch = XMMatrixRotationAxis(m_pTransformCom->Get_State(STATE::RIGHT), m_fPitch); // X축 (상하)
    _matrix matMouseRot = matPitch;

    _matrix     MainBoneMatrix = XMLoadFloat4x4(m_pSocketMatrix1);
    //_matrix     SubBoneMatrix = XMLoadFloat4x4(m_pSocketMatrix2);
    for (size_t i = 0; i < 3; i++)
    {
        MainBoneMatrix.r[i] = XMVector3Normalize(MainBoneMatrix.r[i]);
        //SubBoneMatrix.r[i] = XMVector3Normalize(SubBoneMatrix.r[i]);
    }

    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_DefultWorldMatrix * matMouseRot * MainBoneMatrix * XMLoadFloat4x4(m_pParentMatrix));
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
