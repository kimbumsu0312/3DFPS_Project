#include "pch.h"
#include "LogoCamera.h"

CLogoCamera::CLogoCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CCamera{ pDevice, pContext }
{
}

CLogoCamera::CLogoCamera(const CLogoCamera& Prototype) : CCamera(Prototype)
{
}

HRESULT CLogoCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLogoCamera::Initialize(void* pArg)
{
    CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

    m_fMouseSensor = pDesc->fMouseSensor;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    return S_OK;
}

void CLogoCamera::Priority_Update(_float fTimeDelta)
{
    __super::Update_PipeLines();
}

void CLogoCamera::Update(_float fTimeDelta)
{
}

void CLogoCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CLogoCamera::Render()
{
    return S_OK;
}

void CLogoCamera::Zoom_In(_float fTimeDelta)
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

void CLogoCamera::Zoom_Out(_float fTimeDelta)
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

void CLogoCamera::KeyInput(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyHold(DIK_W))
    {
        m_pTransformCom->Go_Straight(fTimeDelta);
    }

    if (m_pGameInstance->IsKeyHold(DIK_S))
    {
        m_pTransformCom->Go_Backward(fTimeDelta);
    }
    if (m_pGameInstance->IsKeyHold(DIK_A))
    {
        m_pTransformCom->Go_Left(fTimeDelta);
    }
    if (m_pGameInstance->IsKeyHold(DIK_D))
    {
        m_pTransformCom->Go_Right(fTimeDelta);
    }

    if (m_pGameInstance->IsKeyHold(DIK_8))
    {
        m_fFovy += XMConvertToRadians(10.f) * fTimeDelta;
    }

    if (m_pGameInstance->IsKeyHold(DIK_9))
    {
        m_fFovy -= XMConvertToRadians(10.f) * fTimeDelta;
    }
    _int		iMouse = {};
    //이전 마우스 기준으로 몇 픽셀만큼 이동했는지 구분한다.
    if (iMouse = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X))
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * iMouse * m_fMouseSensor);

    if (iMouse = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y))
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), fTimeDelta * iMouse * m_fMouseSensor);
}

CLogoCamera* CLogoCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogoCamera* pInstance = new CLogoCamera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogoCamera"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLogoCamera::Clone(void* pArg)
{
    CLogoCamera* pInstance = new CLogoCamera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLogoCamera"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLogoCamera::Free()
{
    __super::Free();
}
