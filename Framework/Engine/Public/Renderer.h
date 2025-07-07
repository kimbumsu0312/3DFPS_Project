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
	HRESULT						Add_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates, const void* pDesc);
	HRESULT						Switching_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates);
	HRESULT						Draw();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	list<class CGameObject*>	m_RenderObjects[ENUM_CLASS(RENDERGROUP::END)];
	
	_uint						m_iNumRederStates = {};

	map<const _wstring,
		ID3D11DeviceChild*>		m_pRenderState[ENUM_CLASS(RENDERSTATE::END)];

private:
	HRESULT						Render_Priority();
	HRESULT						Render_NonBlend();
	HRESULT						Render_Blend();
	HRESULT						Render_UI();

	ID3D11DeviceChild*			Find_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates);

	HRESULT						Ready_RenderState();
public:
	static CRenderer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};

NS_END