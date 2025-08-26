#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_Tex final : public CUIObject
{
public:
	typedef struct tagInven_Tex_Desc : public CUIObject::UIOBJECT_DESC
	{
		_uint		iTexIndex{};
		_uint		iPassIndex{};
		_wstring	szText{};
		_bool		IsFont =false;
	}UI_TEX_DESC;
private:
	CUI_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Tex(const CUI_Tex& Prototype);
	virtual ~CUI_Tex() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	void						Set_Size(_float2 vSize);
	void						Set_RenderGroup(RENDERGROUP eRenderGroup);
	void						Set_Font(_bool isRender,_wstring szText);
	void						Update_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_int						m_iIndex = {};
	_float						m_fRotation = {};
	_uint						m_iTexIndex = {};
	_uint						m_iPassIndex = {};
	
	_wstring					m_szText = {};
	_bool						m_bIsFont = {};
	RENDERGROUP					m_eRanderGroup = { RENDERGROUP::UI };
	_float						m_fAlpha = { 0.8f};

private:
	HRESULT						Ready_Components();
	void						Render_Font();

public:
	static CUI_Tex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
