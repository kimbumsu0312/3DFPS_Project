#include "pch.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "MonSpawner.h"
#include "Inven_Manager.h"
#include "ItemSpawner.h"

#include "MonState_Normal.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_pGameInstance->OnOff_Light(TEXT("Light_Default_Logo"), false);

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera")))) 
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if(FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Event(TEXT("Layer_Event"))))
		return E_FAIL;

	if (FAILED(Ready_Item()))
		return E_FAIL;

	CInven_Manager::GetInstance()->Level_Init(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Event"));
	m_pGameInstance->On_Static_Shadow(true);
	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	
	if (m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::LB))
	{

		CSpark_Effect::SPARK_EFFECT_INIT SparkDesc;
		SparkDesc.vPos = XMVectorSet(-32.95f, -7.47f, 55.67f, 1.f);
		m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Spark"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), &SparkDesc);

	}
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_1))
		CInven_Manager::GetInstance()->Add_ItemSlot(0, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_2))
		CInven_Manager::GetInstance()->Add_ItemSlot(1, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_3))
		CInven_Manager::GetInstance()->Add_ItemSlot(2, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_4))
		CInven_Manager::GetInstance()->Add_ItemSlot(3, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_5))
		CInven_Manager::GetInstance()->Add_ItemSlot(4, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_6))
		CInven_Manager::GetInstance()->Add_ItemSlot(5, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_7))
		CInven_Manager::GetInstance()->Add_ItemSlot(6, TEXT("Pool_Item"));
	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_8))
		CInven_Manager::GetInstance()->Add_ItemSlot(7, TEXT("Pool_Item"));

	if (m_pGameInstance->IsKeyHold(DIK_LCONTROL) && m_pGameInstance->IsKeyDown(DIK_1))
	{
		if(FAILED(CItemSpawner::GetInstance()->Spawn_Item(5, XMVectorSet(-61.f, -8.5f, 13.5f, 1.f), 0)))
		{
			MSG_BOX(TEXT("아이템 생성 실패"));
		}
	}

	if (m_pGameInstance->IsKeyHold(DIK_LCONTROL) && m_pGameInstance->IsKeyDown(DIK_2))
	{
		if (FAILED(CItemSpawner::GetInstance()->Spawn_Item(6, XMVectorSet(-61.f, -8.5f, 13.5f, 1.f), 0)))
		{
			MSG_BOX(TEXT("아이템 생성 실패"));
		}
	}
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC	LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE::DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.2f, 0.25f, 0.4f, 1.0f);
	LightDesc.vAmbient = _float4(0.05f, 0.1f, 0.15f, 1.0f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.35f, 1.0f);

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Default"), LightDesc)))
		return E_FAIL;

	//다니엘라 방
	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-35.51f, -2.65f, 45.86f, 1.f);
	LightDesc.fRange = 15.f;

	LightDesc.vDiffuse = _float4(0.45f, 0.25f, 0.25f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.15f, 0.15f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Daniela"), LightDesc)))
		return E_FAIL;
	m_pGameInstance->OnOff_Light(TEXT("Light_Daniela"), false);

	//웨어 울프 방
	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-67.32f, -1.65f, 38.54f, 1.f);
	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(0.65f, 0.05f, 0.05f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_WereWolf1"), LightDesc)))
		return E_FAIL;

	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-58.08f, -1.65f, 38.50f, 1.f);
	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(0.65f, 0.05f, 0.05f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_WereWolf2"), LightDesc)))
		return E_FAIL;

	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-48.90f, -1.65f, 38.59f, 1.f);
	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(0.65f, 0.05f, 0.05f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_WereWolf3"), LightDesc)))
		return E_FAIL;

	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-48.87f, -1.65f, 31.35f, 1.f);
	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(0.65f, 0.05f, 0.05f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_WereWolf4"), LightDesc)))
		return E_FAIL;
	m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf1"), false);
	m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf2"), false);
	m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf3"), false);
	m_pGameInstance->OnOff_Light(TEXT("Light_WereWolf4"), false);
	//벨라 방
	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-63.29f, -8.53f, 54.51f, 1.f);
	LightDesc.fRange = 15.f;

	LightDesc.vDiffuse = _float4(0.65f, 0.05f, 0.05f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Bela"), LightDesc)))
		return E_FAIL;
	m_pGameInstance->OnOff_Light(TEXT("Light_Bela"), false);
	//알치나 방
	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-10.07f, -5.27f, 48.96f, 1.f);
	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(1.f, 0.1f, 0.1f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.02f, 0.02f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Alchina"), LightDesc)))
		return E_FAIL;
	m_pGameInstance->OnOff_Light(TEXT("Light_Alchina"), false);
	//세이프 룸
	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-88.98f, -8.52f, 16.94f, 1.f);
	LightDesc.fRange = 7.f;

	LightDesc.vDiffuse = _float4(0.75f, 0.8f, 0.95f, 1.0f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.0f);
	LightDesc.vSpecular = _float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_SafeRoom"), LightDesc)))
		return E_FAIL;

	//플레이어
	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-30.95f, -8.97f, 63.67f, 1.f);
	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(0.75f, 0.8f, 0.95f, 1.0f);
	LightDesc.vAmbient = _float4(0.2f, 0.25f, 0.3f, 1.0f);
	LightDesc.vSpecular = _float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Player"), LightDesc)))
		return E_FAIL;

	LightDesc.eType = LIGHT_DESC::TYPE::POINT;
	LightDesc.vPosition = _float4(-30.95f, -8.97f, 63.67f, 1.f);
	LightDesc.fRange = 20.f;

	LightDesc.vDiffuse = _float4(1.f, 0.6f, 0.2f, 1.0f);
	LightDesc.vAmbient = _float4(0.5f, 0.15f, 0.05f, 1.0f);
	LightDesc.vSpecular = _float4(0.8f, 0.7f, 0.4f, 1.0f);
	if (FAILED(m_pGameInstance->Add_Light(TEXT("Light_Muzzle"), LightDesc)))
		return E_FAIL;
	m_pGameInstance->OnOff_Light(TEXT("Light_Muzzle"), false);

	SHADOW_LIGHT_DESC ShadowLightDesc{};
	ShadowLightDesc.vEye = _float4(10.f, 40.f, -10.f, 1.f);
	ShadowLightDesc.vAt = _float4(-38.69f, -9.10f, 39.28f, 1.f);
	ShadowLightDesc.fFovy = XMConvertToRadians(60.f);
	ShadowLightDesc.fNear = 0.1f;
	ShadowLightDesc.fFar = 100.f;

	if (FAILED(m_pGameInstance->Ready_ShadowLight(ShadowLightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC CameraDesc{};

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

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Level("../Bin/Resources/Data/Level/Level_GamePlay.dat",
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, ENUM_CLASS(LEVEL::GAMEPLAY))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapNevi"))))
		return E_FAIL;

	CGameObject::GAMEOBJECT_DESC DoorDesc = {};
	DoorDesc.fRotationPerSec = 1.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Map_Event_Door"), &DoorDesc)))
		return E_FAIL;

	CSky::SKYBOX_DESC Desc{};
	Desc.vColor = { 0.4f, 0.4f, 0.4f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC Desc;
	Desc.fSpeedPerSec = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = 1.f;
		

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Daniela"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_2"), &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Bela"), &Desc)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Alcina"), &Desc)))
		return E_FAIL;

	CPoolingObject::POOLOBJECT_DESC PoolDesc{};
	PoolDesc.fRotationPerSec = 1.f;
	PoolDesc.fSpeedPerSec = 1.f;
	PoolDesc.szPoolingPath = TEXT("Pool_NormalMon_1");

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1"), 10, &PoolDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	CMuzzle_Effect::MUZZLE_EFFECT_DESC MuzzleHandGunDesc{};

	MuzzleHandGunDesc.szPoolingPath = TEXT("Pool_Muzzle_HandGun");
	MuzzleHandGunDesc.eGunType = CMuzzle_Effect::Gun_Type::HANDGUN;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Effect"), 50, &MuzzleHandGunDesc)))
		return E_FAIL;

	CMuzzle_Effect::MUZZLE_EFFECT_DESC MuzzleShotGunDesc{};

	MuzzleShotGunDesc.szPoolingPath = TEXT("Pool_Muzzle_ShotGun");
	MuzzleShotGunDesc.eGunType = CMuzzle_Effect::Gun_Type::SHOTGUN;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Effect"), 20, &MuzzleShotGunDesc)))
		return E_FAIL;

	CMuzzle_Effect::MUZZLE_EFFECT_DESC MuzzleSniperDesc{};

	MuzzleSniperDesc.szPoolingPath = TEXT("Pool_Muzzle_Sniper");
	MuzzleSniperDesc.eGunType = CMuzzle_Effect::Gun_Type::SNIPER;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Effect"), 20, &MuzzleSniperDesc)))
		return E_FAIL;

	CPoolingObject::POOLOBJECT_DESC BlodeDesc{};

	BlodeDesc.szPoolingPath = TEXT("Pool_Blood");

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blood_Effect"), 100, &BlodeDesc)))
		return E_FAIL;

	CFly_Effect::FLY_EFFECT_DESC FlyDesc{};
	FlyDesc.isDead = true;
	FlyDesc.eType = CFly_Effect::Fly_Type::SPREAD;
	FlyDesc.szPoolingPath = TEXT("Pool_Fly_Spread");
	FlyDesc.fRotationPerSec = 1.f;
	FlyDesc.fSpeedPerSec = 1.f;
	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fly_Effect"), 5, &FlyDesc)))
		return E_FAIL;

	FlyDesc.eType = CFly_Effect::Fly_Type::SPIN;
	FlyDesc.szPoolingPath = TEXT("Pool_Fly_Spin");

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fly_Effect"), 100, &FlyDesc)))
		return E_FAIL;

	FlyDesc.szPoolingPath = TEXT("Pool_Spark");

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Spark_Effect"), 1000, &FlyDesc)))
		return E_FAIL;

	CSnow::SNOW_DESC Desc{};
	Desc.vSnowPos = { -12.26f, -10.46f, 18.5f };
	Desc.vMinPos = { -38.53f, 0.f, 0.f };
	Desc.vMaxPos = { 0.f, 0.f, 37.39f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Snow"), &Desc)))
		return E_FAIL;

	Desc.vSnowPos = { -14.26f, -10.46f, 38.5f };
	Desc.vMinPos = { -28.38f, 0.f, 37.49f };
	Desc.vMaxPos = { 0.f, 0.f, 39.5f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Snow"), &Desc)))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Effect"), &MuzzleDesc)))
	//	return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Pistol"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Shotgun"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Sniper"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_BulletCount"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventroy"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Announce"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Loding_Fade"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Mouse"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Guide_Paper"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Screen_Blood"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Screen_Blood_Particle"))))
		return E_FAIL;
	
	m_pGameInstance->Publish(EVENT_MOUSE_MODE{ {true} });

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Event(const _wstring& strLayerTag)
{
	POOLMONDESC DataPoolMonDesc{};
	DataPoolMonDesc.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
	DataPoolMonDesc.iCellIndex = 7329;
	DataPoolMonDesc.iStartMotion = 1;
	DataPoolMonDesc.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::HALBERD);
	DataPoolMonDesc.szAnimTag = "Sit_Loop";
	DataPoolMonDesc.szState = TEXT("Idle");
	DataPoolMonDesc.vPostion = { -38.69f, -9.10f, 39.28f };
	
	DataPoolMonDesc.vAngleY = 45.f;
	DataPoolMonDesc.iDropImteIndex = 5;
	m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_NormalMon_1"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &DataPoolMonDesc);

	CMonSpawner::MONSPAWNERDESC Desc1{};
	PoolMonDesc MonDesc1{};

	Desc1.vCenter = { 0.f, 0.f, 0.f };
	Desc1.vExtents = { 1.f, 1.f, 1.f };
	Desc1.szPoolPath = TEXT("Pool_NormalMon_1");
	Desc1.vSpawnerPostion = { -37.45f, -8.64f, 27.06f };

	MonDesc1.vPostion = { -34.45f, -8.69f, 27.f };
	MonDesc1.iCellIndex = 3740;
	MonDesc1.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::FALL);
	MonDesc1.szAnimTag = "Fall_Start";
	MonDesc1.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::SWORD);
	MonDesc1.szState = TEXT("Idle");
	MonDesc1.iStartMotion = 3;
	MonDesc1.vAngleY = -90.f;
	MonDesc1.iDropImteIndex = 5;
	Desc1.MonDesc.push_back(MonDesc1);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MonSpawneer"), &Desc1)))
		return E_FAIL;

	CMonSpawner::MONSPAWNERDESC Desc2{};
	PoolMonDesc MonDesc2{};

	Desc2.vCenter = { 0.f, 0.f, 0.f };
	Desc2.vExtents = { 2.f, 2.f, 2.f };
	Desc2.szPoolPath = TEXT("Pool_NormalMon_1");
	Desc2.vSpawnerPostion = { -49.05f, -8.68f, 30.45f };

	//MonDesc2.vPostion = { -64.41f, -8.68f, 27.64f };
	//MonDesc2.iCellIndex = 817;
	//MonDesc2.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
	//MonDesc2.szAnimTag = "Clime_Pop";
	//MonDesc2.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::HALBERD);
	//MonDesc2.szState = TEXT("Idle");
	//MonDesc2.iStartMotion = 2;
	//MonDesc2.vAngleY = -60.f;
	//Desc2.MonDesc.push_back(MonDesc2);

	MonDesc2.vPostion = { -59.54f, -8.68f, 26.96f };
	MonDesc2.iCellIndex = 970;
	MonDesc2.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
	MonDesc2.szAnimTag = "Clime_Pop";
	MonDesc2.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::SHOTEL);
	MonDesc2.szState = TEXT("Idle");
	MonDesc2.iStartMotion = 2;
	MonDesc2.vAngleY = 0.f;
	Desc2.MonDesc.push_back(MonDesc2);

	MonDesc2.vPostion = { -59.53f, -8.67f, 30.00f };
	MonDesc2.iCellIndex = 952;
	MonDesc2.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
	MonDesc2.szAnimTag = "Clime_Pop";
	MonDesc2.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END);
	MonDesc2.szState = TEXT("Idle");
	MonDesc2.iStartMotion = 2;
	MonDesc2.vAngleY = 60.f;
	Desc2.MonDesc.push_back(MonDesc2);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MonSpawneer"), &Desc2)))
		return E_FAIL;

	//MonDesc.vPostion = { -35.82f, -9.06f, 52.25f };
	//MonDesc.iCellIndex = 7430;
	//MonDesc.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
	//MonDesc.szAnimTag = "Sit_Loop";
	//MonDesc.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::HALBERD);
	//MonDesc.szState = TEXT("Idle");
	//MonDesc.iStartMotion = 1;
	//Desc.MonDesc.push_back(MonDesc);

	//MonDesc.vPostion = { -50.29f, -8.68f, 32.24f };
	//MonDesc.iCellIndex = 32;
	//MonDesc.iAnimState = ENUM_CLASS(CMonster_Normal::NORMAL_MON_STATE::NORMAL);
	//MonDesc.szAnimTag = "Idle_Loop";
	//MonDesc.iWeponType = ENUM_CLASS(CMonster_Normal::NORMAL_MON_WEAPON::END);
	//MonDesc.szState = TEXT("Idle");
	//MonDesc.iStartMotion = 1;
	//Desc.MonDesc.push_back(MonDesc);



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Item()
{
	CPoolingObject::POOLOBJECT_DESC Desc{};
	Desc.szPoolingPath = TEXT("Pool_Bullet_HandGun");
	Desc.szModel_Path = TEXT("Prototype_Component_Model_Bullet_HandGun");
	Desc.isLoad = false;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorldItem_Pool"), 20, &Desc)))
		return E_FAIL;

	Desc.szPoolingPath = TEXT("Pool_Bullet_ShotGun");
	Desc.szModel_Path = TEXT("Prototype_Component_Model_Bullet_ShotGun");
	Desc.isLoad = false;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorldItem_Pool"), 20, &Desc)))
		return E_FAIL;

	Desc.szPoolingPath = TEXT("Pool_Bullet_Sniper");
	Desc.szModel_Path = TEXT("Prototype_Component_Model_Bullet_Sniper");
	Desc.isLoad = false;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorldItem_Pool"), 20, &Desc)))
		return E_FAIL;

	Desc.szPoolingPath = TEXT("Pool_Bullet_Potion");
	Desc.szModel_Path = TEXT("Prototype_Component_Model_Potion");
	Desc.isLoad = false;

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorldItem_Pool"), 20, &Desc)))
		return E_FAIL;


	if (FAILED(CItemSpawner::GetInstance()->Level_Init(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Event"))))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();
}
