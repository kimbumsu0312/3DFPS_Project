#include "pch.h"
#include "Imgui_Manager.h"
#include "Edit_Defines.h"
#include "Edit_ModelPath.h"
#include "Terrain.h"
#include "MapObject.h"
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

	if (m_pGameInstance->IsKeyDown(DIK_1) && g_CreateModel)
		g_CreateModel = false;
	else if (m_pGameInstance->IsKeyDown(DIK_1) && !g_CreateModel)
		g_CreateModel = true;

	if (m_pGameInstance->IsKeyDown(DIK_2) && g_SeleteModel)
		g_SeleteModel = false;
	else if (m_pGameInstance->IsKeyDown(DIK_2) && !g_SeleteModel)
		g_SeleteModel = true;


	Begin("Selete Level!");

	if (Button("EDIT_MODEL"))
	{
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MODEL });
		m_eLevel = LEVEL::MODEL;
	}
	End();

	static _int g_iTerrainSizeX = 1024;
	static _int g_iTerrainSizeZ = 1024;
	
	Begin("Create Terrain");

	InputInt("X", &g_iTerrainSizeX);
	InputInt("Z", &g_iTerrainSizeZ);

	if (Button("Create"))
	{
		CVIBuffer_Terrain::VIBUFFER_TERRAIN_DESC Desc{};
		Desc.iNumverticesX = g_iTerrainSizeX;
		Desc.iNumverticesZ = g_iTerrainSizeZ;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MAP), TEXT("Layer_BackGround"), ENUM_CLASS(LEVEL::MAP), TEXT("Prototype_GameObject_Terrain"), &Desc)))
			MSG_BOX(TEXT("Failed to Created : Terrain_Clone"));
	}
	if (Button("Clear"))
	{
		m_pGameInstance->Publish(Clear_Map{});
	}
	Text("Model List");
	if (Combo("##Model", &m_iCurModel_Index, g_ModelPath, IM_ARRAYSIZE(g_ModelPath)))
	{
		m_szModelPath = g_ModelPath[m_iCurModel_Index];
		Text("선택된 항목: %s", g_ModelPath[m_iCurModel_Index]);

	}
	
	Checkbox("Create Model(1)", &g_CreateModel);
	Checkbox("Selete Model(2)", &g_SeleteModel);

	End();


	_float fPos[3] = { m_vPos.x, m_vPos.y, m_vPos.z };
	_float fScale[3] = { m_vScale.x, m_vScale.y, m_vScale.z };
	_float fRot[3] = { m_vRot.x, m_vRot.y, m_vRot.z };
	
	Begin("Selete Model");
	if (nullptr == m_pTransform)
	{
		
		Text("Click to Object");
	}
	else
	{
		Text("NAME: ");
		SameLine();
		Text( m_szSeleteModel);
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
	End();


	if (show_another_window)
	{
		Begin("Another Window", &show_another_window);  
		Text("Hello from another window!");
		if (Button("Close Me"))
			show_another_window = false;
		End();
	}




}

void CImgui_Manger::Update_Model()
{
	Begin("Selete Level!");

	if (Button("EDIT_MAP"))
	{
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MAP });
		m_eLevel = LEVEL::MAP;
	}
	End();

	static _float f = 10.f;
	static _int counter = 0;

	Begin("Model Tool");
	Checkbox("AnotherWindow", &show_another_window);

	SliderFloat("float", &f, 0.0f, 1.0f);

	End();
	if (show_another_window)
	{

		Begin("Another Window", &show_another_window);  
		Text("Hello from another window!");
		if (Button("Close Me"))
			show_another_window = false;
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
	m_pModel = &pObject;

	_wstring szPath = m_pModel->Get_ModelPath();

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
	m_pModel->SetDead();
	m_pModel = nullptr;
	m_pTransform = nullptr;
}

void CImgui_Manger::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
