#pragma once
#include "Base.h"

NS_BEGIN(Edit)

class CImguiDemo final : public CBase
{
private:
	CImguiDemo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImguiDemo() = default;

public:
	HRESULT					Initalize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void					Update();
	HRESULT					Redner();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	
	bool                    m_SwapChainOccluded = false;
	UINT                    m_ResizeWidth = 0, g_ResizeHeight = 0;
	
	// Our state
	bool					show_demo_window = true;
	bool					show_another_window = false;
	ImVec4					clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);

	// Main loop
	bool					done = false;

private:
	void					Test();
public:
	static CImguiDemo*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void			Free();
};
NS_END

