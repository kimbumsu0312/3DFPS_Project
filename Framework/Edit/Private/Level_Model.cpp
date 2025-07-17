#include "pch.h"
#include "Level_Model.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Imgui_Manager.h"
#include "Level_Loading.h"

CLevel_Model::CLevel_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Model::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	m_pGameInstance->Subscribe<Event_NextLevel>([&](const Event_NextLevel& e) {m_bIsNextLevel = true; m_eNextLevel = e.eLevel; });

	return S_OK;
}

void CLevel_Model::Update(_float fTimeDelta)
{
	CImgui_Manger::GetInstance()->Update_Model();
	if (m_bIsNextLevel)
	{
		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, m_eNextLevel))))
			return;
	}
}

HRESULT CLevel_Model::Render()
{
	SetWindowText(g_hWnd, TEXT("¸ðµ¨ ¿¡µ÷"));

	return S_OK;
}

HRESULT CLevel_Model::Ready_Lights()
{
	LIGHT_DESC	LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE::DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Model::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC CameraDesc{};

	CameraDesc.vEye = _float4(0.f, 0.f, -5.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fMouseSensor = 0.2f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MODEL), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Model::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MODEL), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Model::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	return S_OK;
}


CLevel_Model* CLevel_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Model* pInstance = new CLevel_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Model"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Model::Free()
{
	__super::Free();
}
