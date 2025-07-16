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

	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
	return S_OK;
}

void CImgui_Manger::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	Test();
}

HRESULT CImgui_Manger::Redner()
{
	ImGui::Render();


	ImGui_ImplDX11_RenderDrawData(GetDrawData());

	return S_OK;
}

void CImgui_Manger::Test()
{
	if (show_demo_window)
		ShowDemoWindow(&show_demo_window);

	static _float f = 0.f;
	static _int counter = 0;

	Begin("Hellow, World");
	Text("this");
	Checkbox("Demo Window", &show_demo_window);
	Checkbox("AnotherWindow", &show_another_window);

	SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	SameLine();
	Text("counter = %d", counter);

	//Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	End();
	if (show_another_window)
	{
		Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		Text("Hello from another window!");
		if (Button("Close Me"))
			show_another_window = false;
		End();
	}
}

void CImgui_Manger::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
