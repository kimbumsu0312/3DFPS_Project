#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT						Initialize();
	HRESULT						Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT						Draw();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	list<class CGameObject*>	m_RenderObjects[ENUM_CLASS(RENDERGROUP::END)];
	
	ID3D11DepthStencilState*	m_pDepthStencil = { nullptr };
	ID3D11DepthStencilState*	m_pDepthStencil_Off = { nullptr };

	ID3D11BlendState*			m_pAlphablend = { nullptr };
	ID3D11BlendState*			m_pNonAlphablend = { nullptr };


private:
	HRESULT						Render_Priority();
	HRESULT						Render_NonBlend();
	HRESULT						Render_Blend();
	HRESULT						Render_UI();

public:
	static CRenderer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};

NS_END