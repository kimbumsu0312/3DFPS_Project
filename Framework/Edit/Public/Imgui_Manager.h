#pragma once
#include "Base.h"

NS_BEGIN(Edit)

class CImgui_Manger final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manger)
private:
	CImgui_Manger();
	virtual ~CImgui_Manger() = default;

public:
	HRESULT					Initalize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void					Update();
	HRESULT					Redner();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	bool                    m_SwapChainOccluded = false;
	_uint                    m_ResizeWidth = 0, g_ResizeHeight = 0;

	bool					show_demo_window = true;
	bool					show_another_window = false;
	ImVec4					clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
	bool					done = false;

private:
	void					Test();
public:
	virtual void			Free();
};
NS_END

