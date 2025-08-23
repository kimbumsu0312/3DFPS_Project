#include "pch.h"
#include "EditMain.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"
#include "Edit_Model.h"
#include "MapObject.h"
#include "Imgui_Manager.h"
#include "Terrain.h"
#include "VIBuffer_Cube.h"
CEditMain::CEditMain() : m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEditMain::Initialize()
{
	ENGINE_DESC		EngineDesc{};
	
	EngineDesc.hInst = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWinMode = WINMODE::WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::END);
	
	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_ForStatic()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	CImgui_Manger::GetInstance()->Initalize(m_pDevice, m_pContext);

	return S_OK;
}

void CEditMain::Update(_float fTimeDelta)
{
	CImgui_Manger::GetInstance()->Render_Begin();
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CEditMain::Render()
{
	_float4		vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

	m_pGameInstance->Render_Begin(&vClearColor);

	m_pGameInstance->Draw();

	CImgui_Manger::GetInstance()->Redner_End();
	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CEditMain::Ready_Prototype_ForStatic()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* Prototype_Component_Shader_VtxMesh */
   	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	/* Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CEdit_Model::MODEl_DESC Desc{};

	_matrix PreTransformMatrix = XMMatrixIdentity();

	//Desc.szModelName = "Normal_Mon_1";
	//Desc.szModelPath = "Prototype_Component_Model_Normal_Mon_1";
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Normal_Mon_1"),
	//	CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::ANIM, "../Bin/Resources/Models/Normal_Mon_1/Normal_Mon_1.fbx", PreTransformMatrix, &Desc))))
	//	return E_FAIL;
	//
	//m_pGameInstance->Load_Objcet("../Bin/Data/Charter/Player.json", ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Normal_Mon_1"));
	//

	//¸Ê ‰TÀÌÅÍ

	//Desc.szModelName = "BadRoom";
	//Desc.szModelPath = "Prototype_Component_Model_BadRoom";
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_BadRoom"),
	//	CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/Map/Room/badroom/badroom.fbx", PreTransformMatrix, &Desc))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_BadRoom"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/badroom/badroom.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Chapel"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Chapel/Chapel.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Concerthall"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Concerthall/Concerthall.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Courtyardground"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Courtyardground/Courtyardground.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Diningroom"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Diningroom/Diningroom.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Entrancehalla"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Entrancehalla/Entrancehalla.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Kitchen"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Kitchen/Kitchen.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Library"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Library/Library.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Livingroom"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Livingroom/Livingroom.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_MainHall"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/MainHall/MainHall.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_SafeRoom"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/SafeRoom/SafeRoom.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Storageroom"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Storageroom/Storageroom.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TastingRoom"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/TastingRoom/TastingRoom.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_WoodEncorridora"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/WoodEncorridora/WoodEncorridora.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Woodencorridorb"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Woodencorridorb/Woodencorridorb.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Woodencorridorc"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Woodencorridorc/Woodencorridorc.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Woodencorridord"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Woodencorridord/Woodencorridord.json"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Woodencorridore"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Woodencorridore/Woodencorridore.json"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Woodencorridorf"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Room/Woodencorridorf/Woodencorridorf.json"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_DoorFrame"),
		CEdit_Model::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Object/DoorFrame/DoorFrame.json"))))
		return E_FAIL;
	//Desc.szModelName = "Shotel";
	//Desc.szModelPath = "Prototype_Component_Model_Shotel";
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
 // 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Shotel"),
	//	CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/Weapon/Shotel/Shotel.fbx", PreTransformMatrix, &Desc))))
	//	return E_FAIL;

	//Desc.szModelName = "Fiona";
	//Desc.szModelPath = "Prototype_Component_Model_Fiona";
	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fiona"),
	//	CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix, &Desc))))
	//	return E_FAIL;

	//m_pGameInstance->Load_Objcet("../Bin/Data/Charter/Fiona.json", ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fiona"));
	//m_pGameInstance->Load_Objcet("../Bin/Data/Object/ForkLift.json", ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fiona2"));

	//Desc.szModelName = "Player";
	//Desc.szModelPath = "Prototype_Component_Model_Player";
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Player"),
	//	CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::ANIM, "../Bin/Resources/Models/Player/Player.fbx", PreTransformMatrix, &Desc))))
	//	return E_FAIL;

	//Desc.szModelName = "Player";
	//Desc.szModelPath = "Prototype_Component_Model_Player";
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Player"),
	//	CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::ANIM, "../Bin/Resources/Models/Fox/Fox.fbx", PreTransformMatrix, &Desc))))
	//	return E_FAIL;

	/* Prototype_GameObject_Model */

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Model"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	return S_OK;
}

HRESULT CEditMain::Start_Level(LEVEL eStartLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

CEditMain* CEditMain::Create()
{
	CEditMain* pInstance = new CEditMain();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CEditMain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEditMain::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	CImgui_Manger::GetInstance()->Free();
	CImgui_Manger::DestroyInstance();

	m_pGameInstance->Release_Engine();
	
	Safe_Release(m_pGameInstance);
}
