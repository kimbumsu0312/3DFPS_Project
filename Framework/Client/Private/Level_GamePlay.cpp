#include "pch.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "MonSpawner.h"
#include "Inven_Manager.h"
#include "ItemSpawner.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
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

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	_vector vPos = XMVector3Normalize({ 1.f, 1.f, 0.f });
	
	_float f = XMVectorGetX(vPos) + XMVectorGetY(vPos) + XMVectorGetZ(vPos);
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
	LightDesc.vDiffuse = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
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
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Load_Level("../Bin/Resources/Data/Level/Level_GamePlay.dat",
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, ENUM_CLASS(LEVEL::GAMEPLAY))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapNevi"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"))))
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
		
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Bela"), &Desc)))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Daniela"), &Desc)))
		return E_FAIL;


	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Alcina"), &Desc)))
	//	return E_FAIL;

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

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Event(const _wstring& strLayerTag)
{
	CMonSpawner::MONSPAWNERDESC Desc{};
	PoolMonDesc MonDesc{};

	Desc.vCenter = { 0.f, 0.f, 0.f };
	Desc.vExtents = { 1.f, 1.f, 1.f };
	Desc.szPoolPath = TEXT("Pool_NormalMon_1");
	Desc.vSpawnerPostion = { -53.29f, -8.68f, 28.24f };

	MonDesc.vPostion = { -53.29f, -8.68f, 28.24f };
	MonDesc.iCellIndex = 32;
	MonDesc.iAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
	MonDesc.szAnimTag = "Idle_Loop";
	MonDesc.iWeponType = ENUM_CLASS(NORMAL_MON_WEAPON::SWORD);
	MonDesc.szState = TEXT("Stand"); 
	Desc.MonDesc.push_back(MonDesc);

	MonDesc.vPostion = { -50.29f, -8.68f, 28.24f };
	MonDesc.iCellIndex = 32;
	MonDesc.iAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
	MonDesc.szAnimTag = "Idle_Loop";
	MonDesc.iWeponType = ENUM_CLASS(NORMAL_MON_WEAPON::HALBERD);
	MonDesc.szState = TEXT("Stand");
	Desc.MonDesc.push_back(MonDesc);

	MonDesc.vPostion = { -50.29f, -8.68f, 32.24f };
	MonDesc.iCellIndex = 32;
	MonDesc.iAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
	MonDesc.szAnimTag = "Idle_Loop";
	MonDesc.iWeponType = ENUM_CLASS(NORMAL_MON_WEAPON::END);
	MonDesc.szState = TEXT("Stand");
	Desc.MonDesc.push_back(MonDesc);

	MonDesc.vPostion = { -48.29f, -8.68f, 30.24f };
	MonDesc.iCellIndex = 32;
	MonDesc.iAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
	MonDesc.szAnimTag = "Idle_Loop";
	MonDesc.iWeponType = ENUM_CLASS(NORMAL_MON_WEAPON::SHOTEL);
	MonDesc.szState = TEXT("Stand");
	Desc.MonDesc.push_back(MonDesc);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MonSpawneer"), &Desc)))
		return E_FAIL;

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
