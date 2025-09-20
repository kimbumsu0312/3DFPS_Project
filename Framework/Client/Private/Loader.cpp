#include "pch.h"
#include "Loader.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice{ pDevice }, m_pContext { pContext }, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if (FAILED(CoInitializeEx(nullptr, 0)))
	{	
		MSG_BOX(TEXT("Failed to Loading : Loader"));
	}
	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo_Level();
		break;
	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);
	
	if (FAILED(hr))
		return E_FAIL;
	else
		return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Sky/Sky_day0.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/Logo.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Snow/Snow.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Crow/Crow.json", ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Model_Crow"))))
		return E_FAIL;

	/* Prototype_Component_Particle_Snow */
	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		SnowDesc{};
	SnowDesc.iNumInstance = 6000;
	SnowDesc.vCenter = _float3(0.f, 20.f, 0.f);
	SnowDesc.vRange = _float3(50.f, 1.f, 50.f);
	SnowDesc.vSize = _float2(0.2f, 0.1f);
	SnowDesc.vLifeTime = _float2(3.0f, 6.f);
	SnowDesc.vPivot = _float3(0.f, 0.f, 0.f);
	SnowDesc.vSpeed = _float2(7.5f, 10.5f);
	SnowDesc.IsLoop = true;
	SnowDesc.vSplat = _float2(0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Particle_Snow"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &SnowDesc))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/map/Logo/Flower1/Flower1.json", ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Model_Flower1"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/map/Logo/Flower2/Flower2.json", ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Model_Flower2"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/map/Logo/Flower3/Flower3.json", ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Model_Flower3"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/map/Logo/Grass/Grass.json", ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Model_Grass"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트원형를 로딩중입니다."));

	/* Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_UI"),
		CLogo_UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Camera_Logo"),
		CLogoCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Snow"),
		CSnow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Body_Crow"),
		CBody_Crow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Crow"),
		CCrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMap"),
		CLogoMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Hud_Aim"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Bullet/Aim_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));
	
	/* Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		MuzzleDesc{};
	MuzzleDesc.iNumInstance = 8;
	MuzzleDesc.vCenter = _float3(0.f, 0.f, -0.5f);
	MuzzleDesc.vRange = _float3(0.5f, 3.f, 0.5f);
	MuzzleDesc.vSize = _float2(0.5f, 1.3f);
	MuzzleDesc.vLifeTime = _float2(1.f, 1.f);
	MuzzleDesc.vPivot = _float3(0.f, 0.f, 1.f);
	MuzzleDesc.vSpeed = _float2(6.f, 0.8f);
	MuzzleDesc.IsLoop = false;
	MuzzleDesc.vSplat = _float2(0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Muzzle_Smoke_HandGun"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &MuzzleDesc))))
		return E_FAIL;

	MuzzleDesc.iNumInstance = 20;
	MuzzleDesc.vCenter = _float3(0.f, 0.f, -0.5f);
	MuzzleDesc.vRange = _float3(3.f, 2.f, 0.5f);
	MuzzleDesc.vSize = _float2(2.f, 3.f);
	MuzzleDesc.vLifeTime = _float2(1.f, 1.f);
	MuzzleDesc.vPivot = _float3(0.f, 0.f, 1.f);
	MuzzleDesc.vSpeed = _float2(10.f, 10.0f);
	MuzzleDesc.IsLoop = false;
	MuzzleDesc.vSplat = _float2(0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Muzzle_Smoke_ShotGun"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &MuzzleDesc))))
		return E_FAIL;

	MuzzleDesc.iNumInstance = 10;
	MuzzleDesc.vCenter = _float3(0.f, 0.f, -0.5f);
	MuzzleDesc.vRange = _float3(3.f, 2.f, 0.5f);
	MuzzleDesc.vSize = _float2(2.f, 3.f);
	MuzzleDesc.vLifeTime = _float2(1.f, 1.f);
	MuzzleDesc.vPivot = _float3(0.f, 0.f, 1.f);
	MuzzleDesc.vSpeed = _float2(10.f, 10.0f);
	MuzzleDesc.IsLoop = false;
	MuzzleDesc.vSplat = _float2(0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Muzzle_Smoke_Sniper"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &MuzzleDesc))))
		return E_FAIL;

	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		BlodeDesc{};
	BlodeDesc.iNumInstance = 12;
	BlodeDesc.vCenter = _float3(0.f, 0.f, -0.5f);
	BlodeDesc.vRange = _float3(0.25f, 0.25f, 0.f);
	BlodeDesc.vSize = _float2(1.f, 1.f);
	BlodeDesc.vLifeTime = _float2(1.f, 1.f);
	BlodeDesc.vPivot = _float3(0.f, 0.f, 1.f);
	BlodeDesc.vSpeed = _float2(0.5f, 1.0f);
	BlodeDesc.IsLoop = false;
	BlodeDesc.vSplat = _float2(0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Blode_Smoke"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &BlodeDesc))))
		return E_FAIL;

	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		BlodeSplatterDesc{};
	BlodeSplatterDesc.iNumInstance = 20;
	BlodeSplatterDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BlodeSplatterDesc.vRange = _float3(0.1f, 0.1f, 0.f);
	BlodeSplatterDesc.vSize = _float2(0.3f, 0.6f);
	BlodeSplatterDesc.vLifeTime = _float2(0.3f, 0.5f);
	BlodeSplatterDesc.vPivot = _float3(0.f, 0.f, 0.f);
	BlodeSplatterDesc.vSpeed = _float2(0.5f, 1.2f);
	BlodeSplatterDesc.IsLoop = false;
	BlodeSplatterDesc.vSplat = _float2(0.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Blode_Splatter"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &BlodeSplatterDesc))))
		return E_FAIL;

	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		SparkDesc{};
	SparkDesc.iNumInstance = 1;
	SparkDesc.vCenter = _float3(0.f, 0.f, 0.f);
	SparkDesc.vRange = _float3(0.0f, 0.0f, 0.f);
	SparkDesc.vSize = _float2(1.f, 1.f);
	SparkDesc.vLifeTime = _float2(1.f, 1.f);
	SparkDesc.vPivot = _float3(0.1f, 0.f, 0.f);
	SparkDesc.vSpeed = _float2(0.f, 0.f);
	SparkDesc.IsLoop = false;
	SparkDesc.vSplat = _float2(0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Spark"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &SparkDesc))))
		return E_FAIL;

	CVIBuffer_Mesh_Instance::MESH_INSTANCE_DESC FlyDesc{};
	FlyDesc.iNumInstance = 100;
	FlyDesc.vCenter = _float3(0.f, 0.f, 0.f);
	FlyDesc.vRange = _float3(1.f, 1.f, 1.f);
	FlyDesc.vSize = _float2(1.f, 1.f);
	FlyDesc.vLifeTime = _float2(7.f, 10.f);
	FlyDesc.vPivot = _float3(0.f, 0.f, 0.f);
	FlyDesc.vSpeed = _float2(1.f, 1.2f);
	FlyDesc.isLoop = true;
	FlyDesc.vSplat = _float2(0.f, 1.f);

	SAVE_MODEL FlyModelData{};
	m_pGameInstance->Load_ModelData("../Bin/Resources/Models/Effect/Fly/Fly.json", FlyModelData);
		
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_Fly"),
		CModel_Instance::Create(m_pDevice, m_pContext, FlyModelData, &FlyDesc))))
		return E_FAIL;

	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		ScreenBlodeDesc{};
	ScreenBlodeDesc.iNumInstance = 15;
	ScreenBlodeDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ScreenBlodeDesc.vRange = _float3(180.f, 100.f, 0.f);
	ScreenBlodeDesc.vSize = _float2(200.f, 300.f);
	ScreenBlodeDesc.vLifeTime = _float2(0.7f, 1.5f);
	ScreenBlodeDesc.vPivot = _float3(0.f, 0.f, 0.f);
	ScreenBlodeDesc.vSpeed = _float2(0.f, 0.f);
	ScreenBlodeDesc.IsLoop = false;
	ScreenBlodeDesc.vSplat = _float2(0.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_0"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &ScreenBlodeDesc))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_1"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &ScreenBlodeDesc))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_2"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &ScreenBlodeDesc))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_3"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &ScreenBlodeDesc))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Particle_ScreenBlode_4"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &ScreenBlodeDesc))))
		return E_FAIL;
#pragma region Player_Model
	/* Prototype_Model_Player*/
    if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Player/Player.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Player"))))
		return E_FAIL;
	/* Prototype_Model_Knife*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Knife/Knife.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Knife_Weapon"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Knife/Knife_Item.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Knife"))))
		return E_FAIL;

	/* Prototype_Model_HandGun*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/HandGun/HandGun.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_HandGun_Weapon"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/HandGun/HandGun_Item.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_HandGun"))))
		return E_FAIL;

	/* Prototype_Model_ShotGun*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/ShotGun/ShotGun.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_ShotGun_Weapon"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/ShotGun/ShotGun_Item.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_ShotGun"))))
		return E_FAIL;

	/* Prototype_Model_Sniper*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Sniper/Sniper.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Sniper_Weapon"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Sniper/Sniper_Item.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Sniper"))))
		return E_FAIL;
#pragma endregion

#pragma region Mon_Normal_Model
	/* Prototype_Model_Normal_Mon_1*/
  	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Mon/Mon_1/Mon_1.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Normal_Mon_1"))))
		return E_FAIL;
	/* Prototype_Model_Halberd*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Weapon/Halberd/Halberd.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Halberd"))))
		return E_FAIL;
	/* Prototype_Model_Shotel*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Weapon/Shotel/Shotel.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Shotel"))))
		return E_FAIL;
	/* Prototype_Model_Sword*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Weapon/Sword/Sword.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Sword"))))
		return E_FAIL;
	/* Prototype_Model_Normal_Mon_2*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Mon/Mon_2/Mon_2.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Normal_Mon_2"))))
		return E_FAIL;
#pragma endregion
	 
#pragma region Mon_Boss_Model
	/* Prototype_Model_Alcina*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Boss/Alcina/Alcina.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Alcina"))))
		return E_FAIL;

	/* Prototype_Model_Bela*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Boss/Bela/Bela.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Bela"))))
		return E_FAIL;

	/* Prototype_Model_Daniela*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Boss/Daniela/Daniela.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Daniela"))))
		return E_FAIL;

	/* Prototype_Model_Boss_Shotel*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Weapon/Boss_Shotel/Boss_Shotel.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Boss_Shotel"))))
		return E_FAIL;
#pragma endregion

#pragma region MapObject
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Badroom/Badroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BadRoom"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Courtyardground/Courtyardground.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Courtyardground"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Diningroom/Diningroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Diningroom"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Entrancehalla/Entrancehalla.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Entrancehalla"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Livingroom/Livingroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Livingroom"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/MainHall/MainHall.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_MainHall"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/SafeRoom/SafeRoom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_SafeRoom"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/WoodEncorridora/WoodEncorridora.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WoodEncorridora"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Woodencorridorb/Woodencorridorb.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Woodencorridorb"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Chapel/Chapel.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Chapel"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Concerthall/Concerthall.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Concerthall"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Kitchen/Kitchen.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kitchen"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Library/Library.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Library"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Storageroom/Storageroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Storageroom"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/TastingRoom/TastingRoom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_TastingRoom"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Woodencorridorc/Woodencorridorc.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Woodencorridorc"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Woodencorridord/Woodencorridord.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Woodencorridord"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Woodencorridore/Woodencorridore.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Woodencorridore"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Woodencorridorf/Woodencorridorf.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Woodencorridorf"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/DoorFrame/DoorFrame.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_DoorFrame"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Box1/Box1.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Box1"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Box2/Box2.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Box2"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Box3/Box3.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Box3"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/BoxDummy/BoxDummy.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BoxDummy"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Closet/Closet.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Closet"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/DrawerChest/DrawerChest.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_DrawerChest"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Filerack/Filerack.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Filerack"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Kitchencupboard/Kitchencupboard.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kitchencupboard"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/Wardrobe/Wardrobe.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Wardrobe"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/WoodBoard/WoodBoard.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WoodBoard"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/WoodWindow/WoodWindow.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WoodWindow"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/WoodShelf/WoodShelf.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WoodShelf"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Object/WoodTable/WoodTable.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WoodTable"))))
		return E_FAIL;
#pragma endregion
#pragma region ItemObject
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Bullet_HandGun/Bullet_HandGun.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Bullet_HandGun"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Bullet_ShotGun/Bullet_ShotGun.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Bullet_ShotGun"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Bullet_Sniper/Bullet_Sniper.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Bullet_Sniper"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Potion/Potion.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Potion"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Paper/MsgItem_0.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Paper_0"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Paper/MsgItem_1.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Paper_1"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Paper/MsgItem_2.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Paper_2"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Item/Paper/MsgItem_3.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Paper_3"))))
		return E_FAIL;

#pragma endregion
	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩중입니다."));
	/* Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../Bin/Resources/Data/Navigation/Level_GamePlay.dat"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapNevi"),
		CMapNevi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트원형를 로딩중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MonSpawneer"),
		CMonSpawner::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpawnPoint"),
		CSpawnPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
		
#pragma region Player_Object
	if(FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Knife_Player"),
		CKnife::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ShotGun_Player"),
		CShotGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sniper_Player"),
		CSniper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HandGun_Player"),
		CHandGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Monster_Object
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1"),
		CMonster_Normal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1"),
		CBody_NorMon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1_Sword"),
		CNormon_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1_Halberd"),
		CNormon_Halberd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_1_Shotel"),
		CNormon_Shotel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Normal_2"),
		CMonster_WereWolf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_2"),
		CBody_WereWolf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region Monster_Boss_Object
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Alcina"),
		CAlcina::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Alcina"),
		CBody_Alcina::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Bela"),
		CBela::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Bela"),
		CBody_Bela::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Daniela"),
		CDaniela::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Daniela"),
		CBody_Daniela::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Boss_Shotel"),
		CBoss_Shotel::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region UI_Object
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Inventroy"),
		CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Pistol"),
		CAim_Pistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Shotgun"),
		CAim_Shotgun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Sniper"),
		CAim_Sniper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Aim_Line"),
		CAim_Line::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Quick_Slot"),
		CQuick_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Hp"),
		CPlayer_Hp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"),
		CUI_Tex::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Slot"),
		CUI_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Announce"),
		CAnnounce::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_GetUI"),
		CGet_UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Guide_Paper"),
		CGuide_Paper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_BulletCount"),
		CBulletCount::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Screen_Blood"),
		CScreen_Blood::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Screen_Blood_Particle"),
		CScreen_Blood_Particle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Map"),
		CBaseMapObj::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorldItem_Pool"),
		CPoolWorld_Item::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item"),
		CWorldItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MsgItem"),
		CMsgItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trigger"),
		CTrigger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region Effect
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Muzzle_Effect"),
		CMuzzle_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blode_Effect"),
		CBlood_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fly_Effect"),
		CFly_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Spark_Effect"),
		CSpark_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;

}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
