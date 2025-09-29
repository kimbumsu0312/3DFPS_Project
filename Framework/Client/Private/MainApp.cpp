#include "pch.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Fade_UI.h"
#include "Player_Manager.h"
#include "Inven_Manager.h"

#include "Mouse.h"
#include "Mouse_Click_Fx.h"
#include "Animatio_Controller.h"
#include "Item_Slot.h"
#include "Item_Selete.h"
#include "Item_QuickSlot.h"
#include "ItemSpawner.h"

CMainApp::CMainApp() : m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		EngineDesc{};

	EngineDesc.hInst = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWinMode = WINMODE::FULL;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::END);
	EngineDesc.iNumLayerFilter = ENUM_CLASS(COLLISION_LAYER::END);
	EngineDesc.isLut = true;
	EngineDesc.szLutTextureFilePath = TEXT("../Bin/Resources/Textures/Lut/LUT_%d.png");
	EngineDesc.iLutNumTextures = 2;

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Collider()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_ForStatic()))
		return E_FAIL;

	if (FAILED(CPlayer_Manager::GetInstance()->Initialize()))
		return E_FAIL;

	CInven_Manager::INVENTORY_DESC InvenDesc{};
	
	InvenDesc.vInvenCenter = _float2{ g_iWinSizeX * 0.5, g_iWinSizeY * 0.5 + 4};

	InvenDesc.iSlotNumX = 11;
	InvenDesc.iSlotNumY = 7;
	InvenDesc.iSlotSize = 64;

	if (FAILED(CInven_Manager::GetInstance()->Initialize(InvenDesc)))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	D3D11_RASTERIZER_DESC Desc = {};
	 
	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(DIK_ESCAPE))
		DestroyWindow(g_hWnd);
#ifdef _DEBUG

	if (m_pGameInstance->IsKeyDown(DIK_F1))
		m_pGameInstance->IsDebugRender(DEBUG_RENDER::RT);
	if (m_pGameInstance->IsKeyDown(DIK_F2))
		m_pGameInstance->IsDebugRender(DEBUG_RENDER::COMPONET);
#endif // DEBUG

	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	_float4		vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

	m_pGameInstance->Render_Begin(&vClearColor);

	m_pGameInstance->Draw();

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_ForStatic()
{
	//폰트 셋팅
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Godic"), TEXT("../Bin/Resources/Fonts/Godic.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_GangBuJang"), TEXT("../Bin/Resources/Fonts/GangBuJang.spritefont"))))
		return E_FAIL;

	//텍스쳐 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Noies"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Noies/Noies_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Sky/Sky_3.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Trail/Trail_%d.png"), 2))))
		return E_FAIL;

	//버퍼 셋팅
 	/* Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//셰이더 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex_UI.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

   	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_PointParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_PointParticle.hlsl"), VTXPOINTPARTICLE::Elements, VTXPOINTPARTICLE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_Particle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_Particle.hlsl"), VTXPARTICLE::Elements, VTXPARTICLE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_MeshParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_MeshParticle.hlsl"), VTXMESHPARTICLE::Elements, VTXMESHPARTICLE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_Trail"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex_Trail.hlsl"), VTXPOSTEX_TRAIL::Elements, VTXPOSTEX_TRAIL::iNumElements))))
		return E_FAIL;
	//유틸 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
		CAnimatio_Controller::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	//콜라이더 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::AABB))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::OBB))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE))))
		return E_FAIL;

	//UI 셋팅
	if(FAILED(Ready_PrototypeUI_ForStatic()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_PrototypeUI_ForStatic()
{
	//UI 텍스처 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Text_Mouse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Mouse/Mouse_%d.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Item/Item_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Inventory/Inven_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loding"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Loding_Tex/Loding_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loding_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Loding_Tex/Loding_Icon_%d.png"), 1))))
		return E_FAIL;

	//UI 모델 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//UI 오브젝트 셋팅
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_Loding_Icon"),
		CLoding_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_LodingUI"),
		CLodingUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_Loding_Side"),
		CLoding_SideTex::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Loding_Fade"),
		CFade_UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Mouse"),
		CMouse::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Item_Slot"),
		CItem_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Item_Slot_Quick"),
		CItem_QuikSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Item_Slot_Selete"),
		CItem_Selete::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Mouse_Click_Fx"),
		CMouse_Click_Fx::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CMouse_Click_Fx::CLICKFX_CLONE_DESC Desc{};
	Desc.vSize = { 50.f, 50.f };
	Desc.vMinUV = { 0.f , 128.f / 256.f };
	Desc.vMaxUV = { 128.f / 256.f , 256.f / 256.f };
	Desc.szPoolingPath = TEXT("Pool_Click_Fx");

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Object_Mouse_Click_Fx"), 10, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Inven"),
		CInvenItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CPoolingObject::POOLOBJECT_DESC PoolDesc{};
	PoolDesc.fRotationPerSec = 1.f;
	PoolDesc.fSpeedPerSec = 1.f;
	PoolDesc.szPoolingPath = TEXT("Pool_Item");

	if (FAILED(m_pGameInstance->Add_Object_ToPool(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Inven"), 30, &PoolDesc)))
		return E_FAIL;


	return S_OK;

}

HRESULT CMainApp::Start_Level(LEVEL eStartLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Collider()
{
	if(FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::PLAYER), (1 << ENUM_CLASS(COLLISION_LAYER::WEAPON))
											|  (1 << ENUM_CLASS(COLLISION_LAYER::ITEM)))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::KNIFE), (1 << ENUM_CLASS(COLLISION_LAYER::MONSTER)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::RAY), (1 << ENUM_CLASS(COLLISION_LAYER::MONSTER)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::MONSTER), (1 << ENUM_CLASS(COLLISION_LAYER::KNIFE))
										| (1 << ENUM_CLASS(COLLISION_LAYER::RAY) | (1 << ENUM_CLASS(COLLISION_LAYER::PLAYER))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::ITEM), (1 << ENUM_CLASS(COLLISION_LAYER::PLAYER))
										| (1 << ENUM_CLASS(COLLISION_LAYER::PLAYER_VIEW)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::WEAPON), (1 << ENUM_CLASS(COLLISION_LAYER::PLAYER)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::TRIGGER_PLAYER), (1 << ENUM_CLASS(COLLISION_LAYER::PLAYER)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::TRIGGER_MON), (1 << ENUM_CLASS(COLLISION_LAYER::MONSTER)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::RESIST), (1 << ENUM_CLASS(COLLISION_LAYER::RESIST)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Set_LayerFilter(ENUM_CLASS(COLLISION_LAYER::PLAYER_VIEW), (1 << ENUM_CLASS(COLLISION_LAYER::ITEM)))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();
	CPlayer_Manager::GetInstance()->Free();
	CPlayer_Manager::DestroyInstance();
	CInven_Manager::GetInstance()->Free();
	CInven_Manager::DestroyInstance();
	CItemSpawner::GetInstance()->Free();
	CItemSpawner::DestroyInstance();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);

}
