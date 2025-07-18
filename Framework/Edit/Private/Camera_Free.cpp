#include "pch.h"
#include "Camera_Free.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CCamera {pDevice, pContext}
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype) : CCamera(Prototype)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
    KeyInput(fTimeDelta);
    Zoom_In(fTimeDelta);
    Zoom_Out(fTimeDelta);
    __super::Update_PipeLines();
}

void CCamera_Free::Update(_float fTimeDelta)
{
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::Zoom_In(_float fTimeDelta)
{
    if (!m_bIsZoomIn)
        return;

    if(XMConvertToRadians(m_fMoveFovy) < m_fFovy)
        m_fFovy -= XMConvertToRadians(m_fZoomSpeed * fTimeDelta);
    else
    {
        m_fFovy = XMConvertToRadians(m_fMoveFovy);
        m_bIsZoomIn = false;
    }
}

void CCamera_Free::Zoom_Out(_float fTimeDelta)
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

void CCamera_Free::KeyInput(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_Q) && !m_bIsCameraRot)
        m_bIsCameraRot = true;
    else if (m_pGameInstance->IsKeyDown(DIK_Q) && m_bIsCameraRot)
        m_bIsCameraRot = false;

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

    if (m_bIsCameraRot)
    {
        _int		iMouse = {};
        //이전 마우스 기준으로 몇 픽셀만큼 이동했는지 구분한다.
        if (iMouse = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X))
            m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * iMouse * m_fMouseSensor);

        if (iMouse = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y))
            m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), fTimeDelta * iMouse * m_fMouseSensor);
    }
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
    CCamera_Free* pInstance = new CCamera_Free(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCamera_Free::Free()
{
    __super::Free();
}
