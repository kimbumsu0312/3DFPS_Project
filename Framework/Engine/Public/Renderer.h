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
	void						On_Static_Shadow(_bool IsOn);
	_bool						Get_MapShadowOn();

#ifdef _DEBUG
public:
	HRESULT						Add_DebugComponent(class CComponent* pComponent);
	void						IsDebugRender(DEBUG_RENDER eTag);
#endif
private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	list<class CGameObject*>	m_RenderObjects[ENUM_CLASS(RENDERGROUP::END)];

private:
	class CShader*				m_pShader = { nullptr };
	class CShader*				m_pFogShader = { nullptr };
	class CVIBuffer_Rect*		m_pVIBuffer = { nullptr };
	_float4x4					m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};
	
	ID3D11DepthStencilView*		m_pShadowDSV = { nullptr };
	_float						m_fViewportWidth{}, m_fViewportHeight{};

	_bool						m_bIsMapShadow = false;
	
#ifdef _DEBUG
private:
	list<class CComponent*>		m_DebugComponent;
	_bool						m_bRTRender = { false };
	_bool						m_bComponetRender = { false };
#endif // _DEBUG

private:
	HRESULT						Render_Priority();
	HRESULT						Render_Shadow();
	HRESULT						Render_NonBlend();
	HRESULT						Render_Lights();
	HRESULT						Render_Combined();
	HRESULT						Render_NonLight();
	HRESULT						Render_Blend();
	HRESULT						Render_Fog();
	HRESULT						Render_Effect();
	HRESULT						Render_UI();
	HRESULT						Render_UI_Effect();
	HRESULT						Render_Last();
private:
	HRESULT						Ready_Shadow_Depth_Stencil_View();
	HRESULT						SetUp_Viewport(_float fWidth, _float fHeight);

#ifdef _DEBUG
	HRESULT						Render_Debug();
#endif // _DEBUG


public:
	static CRenderer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};

NS_END