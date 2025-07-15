#include "pch.h"
#include "Level_Map.h"
#include "GameInstance.h"
//#include "Camera_Free.h"

CLevel_Map::CLevel_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Map::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Map::Update(_float fTimeDelta)
{
	//TCHAR szChar[MAX_PATH];
	//swprintf_s(szChar, MAX_PATH, L"DeltaTime: %.4f", fTimeDelta);
	//SetWindowText(g_hWnd, szChar);

}

HRESULT CLevel_Map::Render()
{
	SetWindowText(g_hWnd, TEXT("¸Ê ¿¡µ÷"));

	return S_OK;
}

HRESULT CLevel_Map::Ready_Lights()
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

HRESULT CLevel_Map::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	//CCamera_Free::CAMERA_FREE_DESC CameraDesc{};

	//CameraDesc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	//CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	//CameraDesc.fFovy = XMConvertToRadians(60.0f);
	//CameraDesc.fNear = 0.1f;
	//CameraDesc.fFar = 500.f;
	//CameraDesc.fSpeedPerSec = 10.f;
	//CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	//CameraDesc.fMouseSensor = 0.2f;

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_BackGround(const _wstring& strLayerTag)
{


	return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	return S_OK;
}

CLevel_Map* CLevel_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Map* pInstance = new CLevel_Map(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Map"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Map::Free()
{
	__super::Free();
}
