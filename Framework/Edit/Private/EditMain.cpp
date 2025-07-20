#include "pch.h"
#include "EditMain.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"
#include "Edit_Model.h"
#include "MapObject.h"
#include "Imgui_Manager.h"
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
	/* Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	_matrix PreTransformMatrix = XMMatrixIdentity();

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Wall1"),
		CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/Map/Wall/Wall1/Wall1.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Wall2"),
		CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/Map/Wall/Wall2/Wall2.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Wall3"),
		CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/Map/Wall/Wall3/Wall3.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
  	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_ForkLift"),
		CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/ForkLift/Boat.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fiona"),
		CEdit_Model::Create(m_pDevice, m_pContext, MODELTYPE::NONANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix))))
		return E_FAIL;
	/* Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
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
