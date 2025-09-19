#include "pch.h"
#include "Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Obj(TEXT("Layer_Obj"))))
		return E_FAIL;
	//이벤트 구독
	m_pGameInstance->Subscribe<Event_NextLevel>([&](const Event_NextLevel& e) {m_bIsNextLevel = true; });

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (m_bIsNextLevel)
	{
		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::GAMEPLAY))))
			return;
	}

	TCHAR szChar[MAX_PATH];
	swprintf_s(szChar, MAX_PATH, L"DeltaTime: %.4f", fTimeDelta);
	SetWindowText(g_hWnd, szChar);

	return;
}

HRESULT CLevel_Logo::Render()
{
	//SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Lights()
{
	LIGHT_DESC	LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE::DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.0f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.0f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.0f);

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Default_Logo"), LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC CameraDesc{};

	CameraDesc.vEye = _float4(0.f, 0.f, -1.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.6f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fMouseSensor = 0.2f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Camera_Logo"), &CameraDesc)))
		return E_FAIL;

	CameraDesc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	CameraDesc.fMouseSensor = 0.2f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
	//	ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"))))
	//	return E_FAIL;

	CSky::SKYBOX_DESC Desc{};
	Desc.vColor = { 0.07f, 0.08f, 0.1f, 1.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_UI"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Loding_Fade"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Mouse"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	CSnow::SNOW_DESC Desc{};
	Desc.vSnowPos = { 0.f, 0.f, 0.f };
	Desc.vMinPos = { -40.f, 0.f, -40.f };
	Desc.vMaxPos = { 40.f, 0.f, 40.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Snow"), &Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Obj(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Crow"))))
		return E_FAIL;

	CLogoMap::LOGOOBJECT_DESC Desc{};

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Grass");
	Desc.vPos = _float4{ 0.f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 1.f,1.f,1.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;
	
	Desc.szModel_Path = TEXT("Prototype_Component_Model_Grass");
	Desc.vPos = _float4{ -1.f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Grass");
	Desc.vPos = _float4{ 1.f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Flower3");
	Desc.vPos = _float4{ 0.5f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 2.f,2.f,2.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Flower1");
	Desc.vPos = _float4{ -0.3f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 1.f,1.f,1.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Flower2");
	Desc.vPos = _float4{ 0.6f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 1.f,1.f,1.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Flower3");
	Desc.vPos = _float4{ -0.1f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 2.f,2.f,2.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Flower1");
	Desc.vPos = _float4{ -0.8f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 1.f,1.f,1.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;

	Desc.szModel_Path = TEXT("Prototype_Component_Model_Flower2");
	Desc.vPos = _float4{ -0.5f, 0.f, 0.f, 1.f };
	Desc.fDegrees = 0.f;
	Desc.vScale = _float3{ 1.5f,1.5f,1.5f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"), &Desc)))
		return E_FAIL;
	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
