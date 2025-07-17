#include "pch.h"
#include "Imgui_Manager.h"
#include "Edit_Defines.h"

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
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MAP });

	SameLine();

	if (Button("EDIT_MODEL"))
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MODEL });

	End();

}

void CImgui_Manger::Update_Map()
{
	Begin("Selete Level!");

	if (Button("EDIT_MODEL"))
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MODEL });

	End();

	static _float f = 10.f;
	static _int counter = 0;

	Begin("Map Tool!");
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

void CImgui_Manger::Update_Model()
{
	Begin("Selete Level!");

	if (Button("EDIT_MAP"))
		m_pGameInstance->Publish(Event_NextLevel{ LEVEL::MAP });

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

void CImgui_Manger::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
