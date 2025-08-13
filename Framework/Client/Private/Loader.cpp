#include "pch.h"
#include "Loader.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Camera_Free.h"
#include "Sky.h"

#include "UI_Header.h"
#include "Player_Header.h"

#include "Monster_Normal.h"
#include "Body_NorMon.h"
#include "NorMon_Sword.h"
#include "NorMon_Halberd.h"
#include "NorMon_Shotel.h"
#include "BaseMapObj.h"

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

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT(" 이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트원형를 로딩중입니다."));

	/* Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_UI"),
		CLogo_UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Inventory/Inven_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Hud_Aim"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Bullet/Aim_%d.png"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Sky/Sky_3.dds"), 1))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));
	
	/* Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;
	/* Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region Player_Model
	/* Prototype_Model_Player*/
    if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Player/Player.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Player"))))
		return E_FAIL;
	/* Prototype_Model_Knife*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Knife/Knife.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Knife"))))
		return E_FAIL;
	/* Prototype_Model_HandGun*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/HandGun/HandGun.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_HandGun"))))
		return E_FAIL;
	/* Prototype_Model_ShotGun*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/ShotGun/ShotGun.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_ShotGun"))))
		return E_FAIL;
	/* Prototype_Model_Sniper*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Sniper/Sniper.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Sniper"))))
		return E_FAIL;
#pragma endregion

#pragma region Mon_Normal_Model
	/* Prototype_Model_Normal_Mon_1*/
  	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Normal_Mon_1/Normal_Mon_1.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Normal_Mon_1"))))
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
#pragma endregion

#pragma region MapObject
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Badroom/Badroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BadRoom"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Courtyardground/Courtyardground.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Courtyardground"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Diningroom/Diningroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Diningroom"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Entrancehalla/Entrancehalla.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Entrancehalla"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Livingroom/Livingroom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Livingroom"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/MainHall/MainHall.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_MainHall"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/SafeRoom/SafeRoom.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_SafeRoom"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/WoodEncorridora/WoodEncorridora.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WoodEncorridora"))))
		return E_FAIL;
	/* Prototype_Model_Badroom*/
	if (FAILED(m_pGameInstance->Load_Objcet("../Bin/Resources/Models/Map/Room/Woodencorridorb/Woodencorridorb.json", ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Woodencorridorb"))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/* Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트원형를 로딩중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1_Sword"),
		CNormon_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1_Halberd"),
		CNormon_Halberd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1_Shotel"),
		CNormon_Shotel::Create(m_pDevice, m_pContext))))
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

#pragma endregion
	/* Prototype_GameObject_BaseMapObj */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BaseMapObj"),
		CBaseMapObj::Create(m_pDevice, m_pContext))))
		return E_FAIL;
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
