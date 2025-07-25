#include "pch.h"
#include <Windows.h>
#include <commdlg.h> 
#include "Imgui_Manager.h"
#include "Edit_Defines.h"
#include "Edit_ModelPath.h"
#include "Terrain.h"
#include "MapObject.h"
#include "Engine_Defines.h"
#include "Edit_Model.h"
IMPLEMENT_SINGLETON(CImgui_Manger)

CImgui_Manger::CImgui_Manger()
{
}

HRESULT CImgui_Manger::Initalize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();

	CreateContext();

	ImGuiIO& io = GetIO();
	(void)io;

	io.ConfigFlags != ImGuiConfigFlags_NavEnableKeyboard;

	StyleColorsDark;

	ImGui_ImplWin32_Init((HWND)g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	m_pGameInstance = CGameInstance::GetInstance();
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
	m_szModelPath = g_ModelPath[m_iCurModel_Index];
	
	D3D11_RASTERIZER_DESC Desc = {};

	Desc.FillMode = D3D11_FILL_WIREFRAME;
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FrontCounterClockwise = false;
	Desc.DepthClipEnable = true;

	pDevice->CreateRasterizerState(&Desc, &m_pWireframeRS);

	Desc.FillMode = D3D11_FILL_SOLID;
	Desc.CullMode = D3D11_CULL_BACK;
	Desc.FrontCounterClockwise = false;
	Desc.DepthClipEnable = true;
	pDevice->CreateRasterizerState(&Desc, &m_pSolidframeRS);

	return S_OK;
}

void CImgui_Manger::Render_Begin()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
}

void CImgui_Manger::Update_Logo()
{
	Begin("Selete Level!");

	if (Button("EDIT_MAP"))
	{
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MAP });
		m_eLevel = LEVEL::MAP;
	}
	SameLine();

	if (Button("EDIT_MODEL"))
	{
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MODEL });
		m_eLevel = LEVEL::MODEL;
	}
	End();

}

void CImgui_Manger::Update_Map()
{

	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_1) && g_CreateModel)
		g_CreateModel = false;
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_1) && !g_CreateModel)
		g_CreateModel = true;

	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_2) && g_SeleteModel)
		g_SeleteModel = false;
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_2) && !g_SeleteModel)
		g_SeleteModel = true;

	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_3) && g_MoveModel)
		g_MoveModel = false;
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_3) && !g_MoveModel)
		g_MoveModel = true;

	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_4) && g_TerrainHight)
		g_TerrainHight = false;
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_4) && !g_TerrainHight)
		g_TerrainHight = true;

	if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_5) && m_bISWireFream)
	{
		m_pContext->RSSetState(m_pSolidframeRS);
		m_bISWireFream = false;
	}
	else if (m_pGameInstance->IsKeyHold(DIK_LSHIFT) && m_pGameInstance->IsKeyDown(DIK_5) && !m_bISWireFream)
	{
		m_pContext->RSSetState(m_pWireframeRS);
		m_bISWireFream = true;
	}
	static _int g_iTerrainSizeX = 200;
	static _int g_iTerrainSizeZ = 200;
	
	if (BeginTabBar("옵션"))
	{
		if (BeginTabItem("Terrain"))
		{
			if (Checkbox("WireFrameMode(5)", &m_bISWireFream))
				m_pContext->RSSetState(m_bISWireFream ? m_pWireframeRS : m_pSolidframeRS);

			InputInt("X", &g_iTerrainSizeX);
			InputInt("Z", &g_iTerrainSizeZ);


			if (Button("Create"))
			{

				CTerrain::TERRAIN_DESC Desc;
				Desc.iNumverticesX = g_iTerrainSizeX;
				Desc.iNumverticesZ = g_iTerrainSizeZ;
				Desc.Terrain_Data = nullptr;
				Desc.szModel_Path = TEXT("Terrain");
				
				CTerrain* pTerrain = CTerrain::Create(m_pDevice, m_pContext, &Desc);

				if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MAP), TEXT("Layer_BackGround"), ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"), pTerrain)))
					MSG_BOX(TEXT("Failed to Created : Terrain"));
				m_pTerrain = pTerrain;
				Safe_AddRef(m_pTerrain);

			}
			if (Button("Clear"))
			{
				m_pGameInstance->Publish(Clear_Map{});
				Safe_Release(m_pTerrain);
				m_pGameInstance->Clear_Object();
			}
			Text("Model List");
			if (Combo("##Model", &m_iCurModel_Index, g_ModelPath, IM_ARRAYSIZE(g_ModelPath)))
			{
				m_szModelPath = g_ModelPath[m_iCurModel_Index];
				Text("선택된 항목: %s", g_ModelPath[m_iCurModel_Index]);

			}

			Checkbox("Create Model(1)", &g_CreateModel);
			Checkbox("Selete Model(2)", &g_SeleteModel);
			Checkbox("Move Model(3)", &g_MoveModel);
			Checkbox("Terrain Height(4)", &g_TerrainHight);
	
#pragma region TERRAIN HEIGHT
			Text("BrashSize");
			SameLine();
			if (Button("-##BrashSize")) m_fBrash -= 0.1f;
			SameLine();
			InputFloat("##BrashSize", &m_fBrash);
			SameLine();
			if (Button("+##BrashSize")) m_fBrash += 0.1f;

			Text("Height");
			SameLine();
			if (Button("-##Height")) m_fHeight -= 0.1f;
			SameLine();
			InputFloat("##Height", &m_fHeight);
			SameLine();
			if (Button("+##Height")) m_fHeight += 0.1f;
			
			Text("MinHegith");
			SameLine();
			if (Button("-##MinHegith")) m_fMinMaxHeight.x -= 0.1f;
			SameLine();
			InputFloat("##MinHegith", &m_fMinMaxHeight.x);
			SameLine();
			if (Button("+##MinHegith")) m_fMinMaxHeight.x += 0.1f;

			Text("MaxHeight");
			SameLine();
			if (Button("-##MaxHeight")) m_fMinMaxHeight.y -= 0.1f;
			SameLine();
			InputFloat("##MaxHeight", &m_fMinMaxHeight.y);
			SameLine();
			if (Button("+##MaxHeight")) m_fMinMaxHeight.y += 0.1f;

			Checkbox("HeightUP", &m_bIsHeightUP);
#pragma endregion
			EndTabItem();

		}

		if (BeginTabItem("Model"))
		{
			_float fPos[3] = { m_vPos.x, m_vPos.y, m_vPos.z };
			_float fScale[3] = { m_vScale.x, m_vScale.y, m_vScale.z };
			_float fRot[3] = { m_vRot.x, m_vRot.y, m_vRot.z };

			if (nullptr == m_pTransform)
				Text("Click to Object");
			else
			{
				Text("NAME: ");	SameLine();	Text(m_szSeleteModel);
				Checkbox("Move Model(3)", &g_MoveModel);
#pragma region POSITION
				Text("POSITION");
				Text("X");
				SameLine();
				if (Button("-##POSITIONX")) fPos[0] -= 0.1f;
				SameLine();
				InputFloat("##POSITIONX", &fPos[0]);
				SameLine();
				if (Button("+##POSITIONX")) fPos[0] += 0.1f;

				Text("Y");
				SameLine();
				if (Button("-##POSITIONY")) fPos[1] -= 0.1f;
				SameLine();
				InputFloat("##POSITIONY", &fPos[1]);
				SameLine();
				if (Button("+##POSITIONY")) fPos[1] += 0.1f;

				Text("Z");
				SameLine();
				if (Button("-##POSITIONZ")) fPos[2] -= 0.1f;
				SameLine();
				InputFloat("##POSITIONZ", &fPos[2]);
				SameLine();
				if (Button("+##POSITIONZ")) fPos[2] += 0.1f;

				if (fPos[0] != m_vPos.x || fPos[1] != m_vPos.y || fPos[2] != m_vPos.z)
					PosSet(_float4{ fPos[0], fPos[1], fPos[2], 1.f });
#pragma endregion
#pragma region SCALE
				Text("SCALE");
				Text("X");
				SameLine();
				if (Button("-##SCALEX")) fScale[0] -= 0.1f;
				SameLine();
				InputFloat("##SCALEX", &fScale[0]);
				SameLine();
				if (Button("+##SCALEX")) fScale[0] += 0.1f;
				Text("Y");
				SameLine();
				if (Button("-##SCALEY")) fScale[1] -= 0.1f;
				SameLine();
				InputFloat("##SCALEY", &fScale[1]);
				SameLine();
				if (Button("+##SCALEY")) fScale[1] += 0.1f;
				Text("Z");
				SameLine();
				if (Button("-##SCALEZ")) fScale[2] -= 0.1f;
				SameLine();
				InputFloat("##SCALEZ", &fScale[2]);
				SameLine();
				if (Button("+##SCALEZ")) fScale[2] += 0.1f;

				if (fScale[0] != m_vScale.x || fScale[1] != m_vScale.y || fScale[2] != m_vScale.z)
					ScaleSet(_float3{ fScale[0], fScale[1], fScale[2] });

#pragma endregion
#pragma region Rotation
				Text("Rotation");
				Text("X");
				SameLine();
				if (Button("-##RotationX")) fRot[0] -= 0.01f;
				SameLine();
				InputFloat("##RotationX", &fRot[0]);
				SameLine();
				if (Button("+##RotationX")) fRot[0] += 0.01f;
				Text("Y");
				SameLine();
				if (Button("-##RotationY")) fRot[1] -= 0.01f;
				SameLine();
				InputFloat("##RotationY", &fRot[1]);
				SameLine();
				if (Button("+##RotationY")) fRot[1] += 0.01f;
				Text("Z");
				SameLine();
				if (Button("-##RotationZ")) fRot[2] -= 0.01f;
				SameLine();
				InputFloat("##RotationZ", &fRot[2]);
				SameLine();
				if (Button("+##RotationZ")) fRot[2] += 0.01f;


				if (fRot[0] != m_vRot.x || fRot[1] != m_vRot.y || fRot[2] != m_vRot.z)
					RotSet(_float3{ fRot[0], fRot[1], fRot[2] });
#pragma endregion

				if (Button("Erase"))
					Erase_Model();
			}
			EndTabItem();
		}

		if (BeginTabItem("Level"))
		{
			if (Button("EDIT_MODEL"))
			{
				m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MODEL });
				m_eLevel = LEVEL::MODEL;
			}
			EndTabItem();
		}

		if (BeginTabItem("SaveLoad"))
		{
#pragma region TERRAIN_SAVELOAD
			InputText("파일 이름: ", m_szFileName, MAX_PATH);
			if (Button("Terrain Save"))
			{
				if (m_pTerrain != nullptr && m_szFileName != "")
				{
					string szFileName = m_szFileName;
					m_pGameInstance->File_Save_TerrainLevel(DATA_TYPE::TERRAIN, szFileName, m_pTerrain->Get_Buffer() );
				}
				else
					MSG_BOX(TEXT("세이브 실패"));
			}
			SameLine();
			if (Button("Terrain Load"))
			{
				if (m_szFileName != "")
				{
					m_pGameInstance->Publish(Clear_Map{});
					Safe_Release(m_pTerrain);

					string szFileName = m_szFileName;
					SAVE_TERRAIN pData = {};
					m_pGameInstance->Load_Terrain(szFileName, pData);

					CTerrain::TERRAIN_DESC Desc;
					Desc.iNumverticesX = g_iTerrainSizeX;
					Desc.iNumverticesZ = g_iTerrainSizeZ;
					Desc.Terrain_Data = &pData;
					Desc.szModel_Path = TEXT("Terrain");

					CTerrain* pTerrain = CTerrain::Create(m_pDevice, m_pContext, &Desc);

					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MAP), TEXT("Layer_BackGround"), ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Terrain"), pTerrain)))
						MSG_BOX(TEXT("Failed to Created : Terrain"));
					m_pTerrain = pTerrain;
					Safe_AddRef(m_pTerrain);
				}
				else
					MSG_BOX(TEXT("로드 실패"));
			}
#pragma endregion
#pragma region LEVEL_SAVELOAD
			if (Button("Level Save"))
			{
				if (m_szFileName != "")
				{
					string szFileName = m_szFileName;
					m_pGameInstance->File_Save_TerrainLevel(DATA_TYPE::LEVEL, szFileName, nullptr);
				}
				else
					MSG_BOX(TEXT("세이브 실패"));
			}
			SameLine();
			if (Button("Level Load"))
			{
				if (m_szFileName != "")
				{
					string szFileName = m_szFileName;
					m_pGameInstance->Load_Level(szFileName, ENUM_CLASS(LEVEL::MAP), TEXT("Layer_Object"), ENUM_CLASS(LEVEL::STATIC));
				}
				else
					MSG_BOX(TEXT("로드 실패"));
			}
#pragma endregion
			EndTabItem();
		}
		EndTabBar();
	}

}

void CImgui_Manger::Update_Model()
{
	static _int		g_iEreaseData = {};
	static _char	g_szAnimName[MAX_PATH] = {};
	static _int		g_iStartFrame = {};
	static _int		g_iEndFrame = {};
	static _float   g_fTickPerSecond = {};
	static _float   g_SetTick = {};
	static _float   g_SetCulFream = {};
	static char		g_AnimFilePath = {};
	static _float   g_CurTrackPositon = {};
	static _float   g_Duration = {};
	Begin("ModelData");
	if (BeginTabBar("옵션"))
	{
		if (BeginTabItem("Model"))
		{
			if (Combo("##Model", &m_iCurModel_Index, g_ModelPath, IM_ARRAYSIZE(g_ModelPath)))
			{
				m_szModelPath = g_ModelPath[m_iCurModel_Index];
				Text("선택된 항목: %s", g_ModelPath[m_iCurModel_Index]);
			}

			if(Button("Create"))
			{
				if(m_pModel != nullptr)
					Erase_Model();
				CMapObject::MODEL_OBJECT_DESC Desc{};
				Desc.vPos = { 0.f, 0.f, 0.f, 1.f };
				Desc.szModel_Path = Get_ModelPath();
				Desc.szObject_Path = TEXT("Prototype_GameObject_Model");

				CMapObject* pInstance = static_cast<CMapObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Model"), &Desc));
				if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MODEL), TEXT("Layer_Model"), ENUM_CLASS(LEVEL::MODEL), TEXT("Prototype_GameObject_Model"), pInstance)))
					MSG_BOX(TEXT("Failed to Created : Model"));
				
				if (pInstance != nullptr)
				{
					m_pModel = pInstance;
					Safe_AddRef(m_pModel);
					m_pModelCom = m_pModel->Get_Model();
				}
			}
			EndTabItem();
		}

		if (BeginTabItem("Animation"))
		{
			if (m_pModel != nullptr)
			{
				g_CurTrackPositon = m_pModelCom->Get_Animation(1);
				g_Duration = m_pModelCom->Get_Animation(2);
				Text("TickPerSecond : %.2f", m_pModelCom->Get_Animation(0));
				Text("CurTrackPositon : %.2f", g_CurTrackPositon);

				if (SliderFloat("##AnimSlider", &g_CurTrackPositon, 0.f, g_Duration))
					m_pModelCom->Set_Animation(1, 0.f, g_CurTrackPositon);

				if (Button("TickPerSecondAdd"))
					m_pModelCom->Set_Animation(0, g_SetTick, 0.f);
				SameLine();
				InputFloat("##TickPerSecondAddValue", &g_SetTick);

				if (Button("CurTrackPositonAdd"))
					m_pModelCom->Set_Animation(1, 0.f, g_SetCulFream);
				SameLine();
				InputFloat("##CurTrackPositonAddValue", &g_SetCulFream);

				if (Button("-(z)##Frame") || m_pGameInstance->IsKeyHold(DIK_Z))
					m_pModelCom->Set_Animation(2, 0.f, -1);
				SameLine();
				if (Button("Stop(x)") || m_pGameInstance->IsKeyHold(DIK_X))
					m_pModel->Set_AnimStop();

				SameLine();
				if (Button("+(c)##Frame") || m_pGameInstance->IsKeyHold(DIK_C))
					m_pModelCom->Set_Animation(2, 0.f, 1);

				InputText("AnimName", g_szAnimName, MAX_PATH);
				InputInt("StartFrame", &g_iStartFrame);
				InputInt("ENDFrame", &g_iEndFrame);
				InputFloat("TickPerSecond", &g_fTickPerSecond);

				if (Button("DataUpdate"))
				{
					SAVE_ANIMDATA Data{};
					Data.szAnimName = g_szAnimName;
					Data.iStartFrame = g_iStartFrame;
					Data.iEndFrame = g_iEndFrame;
					Data.fTickPerSecond = g_fTickPerSecond;

					m_SaveAnimData.push_back(Data);
				}
			}
			EndTabItem();
		}

	
		EndTabBar();
	}
	End();


	if (m_SaveAnimData.size() != 0)
	{
		Begin("AnimationTable");
		if (BeginTable("##AnimationTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			// 컬럼 헤더 설정
			TableSetupColumn("Num");
			TableSetupColumn("Name");
			TableSetupColumn("Start Frame");
			TableSetupColumn("End Frame");
			TableSetupColumn("TickPerSecond");
			TableHeadersRow();
			_int i = 0;
			// 각 행에 데이터 출력
			for (const auto& Data : m_SaveAnimData)
			{
				TableNextRow();
				TableSetColumnIndex(0);
				Text("%d", i);

				TableSetColumnIndex(1);
				Text("%s", Data.szAnimName.c_str());

				TableSetColumnIndex(2);
				Text("%u", Data.iStartFrame);

				TableSetColumnIndex(3);
				Text("%u", Data.iEndFrame);

				TableSetColumnIndex(4);
				Text("%.2f", Data.fTickPerSecond);
				++i;
			}
			EndTable();

			InputInt("##EraseIndex", &g_iEreaseData);	
			SameLine();
			if(Button("erease"))
			{ 
				if (g_iEreaseData < m_SaveAnimData.size())
					m_SaveAnimData.erase(m_SaveAnimData.begin() + g_iEreaseData);
			}

			InputText("##jsonDataSave ", m_szFileName, MAX_PATH);
			SameLine();
			if (Button("DataSave"))
			{
				if (m_SaveAnimData.size() != 0 && m_szFileName != "")
				{
					string szFileName = m_szFileName;
					m_pGameInstance->File_Save_AnimData(m_szFileName, m_SaveAnimData);
				}
				else
					MSG_BOX(TEXT("세이브 실패"));
			}
		}
		End();
	}


}

HRESULT CImgui_Manger::Redner_End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(GetDrawData());

	return S_OK;
}

void CImgui_Manger::Selete_Object(CMapObject& pObject, CTransform& pTransform)
{
	if (m_pModel != nullptr)
		m_pModel->SetSelete(false);

	m_pModel = &pObject;
	m_pModel->SetSelete(true);

	_wstring szPath = TEXT("123");

	WideCharToMultiByte(CP_UTF8, 0,	szPath.c_str(), -1,	m_szSeleteModel, MAX_PATH, NULL, NULL );
	
	m_pTransform = &pTransform;

	_vector vScale{}, vRot{}, vPos{};
	XMMatrixDecompose(&vScale, &vRot, &vPos, m_pTransform->Get_WorldMatrix());

	_float4 vRot4;
	XMStoreFloat4(&vRot4, vRot);

	XMStoreFloat3(&m_vPos, vPos);
	XMStoreFloat3(&m_vScale, vScale);

	m_vRot.x = atan2f(2.f * (vRot4.w * vRot4.x + vRot4.y * vRot4.z), 1.f - 2.f * (vRot4.x * vRot4.x + vRot4.y * vRot4.y));
	m_vRot.y = asinf(2.f * (vRot4.w * vRot4.y - vRot4.z * vRot4.x));
	m_vRot.z = atan2f(2.f * (vRot4.w * vRot4.z + vRot4.x * vRot4.y), 1.f - 2.f * (vRot4.y * vRot4.y + vRot4.z * vRot4.z));

	m_vRot.x = XMConvertToDegrees(m_vRot.x);
	m_vRot.y = XMConvertToDegrees(m_vRot.y);
	m_vRot.z = XMConvertToDegrees(m_vRot.z);
}

void CImgui_Manger::PosSet(_float4 fPos)
{
	m_pTransform->Set_State(STATE::POSITION, XMLoadFloat4(&fPos));
	m_vPos.x = fPos.x;
	m_vPos.y = fPos.y;
	m_vPos.z = fPos.z;
}

void CImgui_Manger::ScaleSet(_float3 fScale)
{
	m_pTransform->Scale(fScale);
	m_vScale = fScale;
}

void CImgui_Manger::RotSet(_float3 fRot)
{
	m_pTransform->Rotation_All(fRot);

	m_vRot = fRot;
}

_wstring CImgui_Manger::Get_ModelPath()
{
	int size_needed = MultiByteToWideChar(
		CP_UTF8,            // UTF-8 인코딩
		0,                  // 플래그 (기본값)
		m_szModelPath.c_str(),        // 입력 문자열
		(int)m_szModelPath.size(),    // 문자열 길이
		NULL, 0             // NULL일 때 크기만 계산
	);

	// 버퍼 할당 및 변환
	wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, m_szModelPath.c_str(), (int)m_szModelPath.size(), &wstr[0], size_needed);

	return wstr;
}

void CImgui_Manger::Erase_Model()
{
	m_pGameInstance->Publish(Event_Erase_Model{});

	m_pModel->SetDead();
	m_pModel = nullptr;
	m_pTransform = nullptr;
	m_pModelCom = nullptr;
}

void CImgui_Manger::Move_Model(_float3 fPos)
{
	if (m_pModel == nullptr)
		return;

	
	PosSet(_float4{ fPos.x,fPos.y,fPos.z, 1.f });

	_vector vScale{}, vRot{}, vPos{};
	XMMatrixDecompose(&vScale, &vRot, &vPos, m_pTransform->Get_WorldMatrix());

	XMStoreFloat3(&m_vPos, vPos);
}

void CImgui_Manger::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
	m_pModelCom = nullptr;
	m_pTransform = nullptr;
	Safe_Release(m_pModel);
	Safe_Release(m_pWireframeRS);
	Safe_Release(m_pSolidframeRS);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTerrain);
}
